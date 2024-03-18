#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/compute.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/command_buffer.hpp>
#include <gct/exception.hpp>
#include <gct/texture_pool.hpp>
namespace gct {

texture_pool::texture_index_t texture_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= texture_state.size() ) {
    texture_state.resize( index + 1u );
  }
  return index; 
}

void texture_pool::state_type::release_index( texture_pool::texture_index_t index ) {
  if( index >= texture_state.size() || !texture_state[ index ].valid ) {
    return;
  }
  texture_state[ index ].valid = false;
  index_allocator.release( index );
}

texture_pool::views texture_pool::state_type::allocate(
  const std::string &filename,
  bool mipmap, integer_attribute_t attr,
  unsigned int max_channels_per_layer
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  const texture_index_t normalized_index = allocate_index();
  const texture_index_t scaled_index = allocate_index();
  const texture_index_t srgb_index = allocate_index();

  write_request_list.push_back(
    write_request()
      .set_normalized_index( normalized_index )
      .set_scaled_index( scaled_index )
      .set_srgb_index( srgb_index )
      .set_filename( filename )
      .set_mipmap( mipmap )
      .set_attr( attr )
      .set_max_channels_per_layer( max_channels_per_layer )
  );
  texture_state[ normalized_index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u );

  texture_descriptor normalized_desc(
    new texture_index_t( normalized_index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( normalized_desc );

  texture_state[ scaled_index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u );

  texture_descriptor scaled_desc(
    new texture_index_t( scaled_index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( scaled_desc );

  texture_state[ srgb_index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u );

  texture_descriptor srgb_desc(
    new texture_index_t( srgb_index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( srgb_desc );

  return views()
    .set_normalized( normalized_desc )
    .set_scaled( scaled_desc )
    .set_srgb( srgb_desc );
}

void texture_pool::state_type::release( texture_index_t index ) {
  texture_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( texture_state.size() <= index || !texture_state[ index ].valid ) {
      return;
    }
    removed = texture_state[ index ];
    release_index( index );
    const auto target = (*props.descriptor_set)[ props.descriptor_name ];
    std::vector< write_descriptor_set_t > updates;
    updates.push_back(
      write_descriptor_set_t()
        .set_basic(
          vk::WriteDescriptorSet( target )
            .setDstArrayElement( index )
            .setDescriptorCount( 1u )
        )
        .add_image( null_view )
    );
    props.descriptor_set->update(
      std::move( updates )
    );
  }
}

texture_pool::state_type::state_type( const texture_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_texture_count ) ) {
}

void texture_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  std::vector< write_descriptor_set_t > updates;
  const auto target = (*props.descriptor_set)[ props.descriptor_name ];
  for( const auto &req: write_request_list ) {
    auto image = rec.load_image(
      props.allocator,
      req.filename,
      vk::ImageUsageFlagBits::eSampled,
      req.mipmap,
      req.attr,
      req.max_channels_per_layer
    );
    for( auto f: image->get_props().get_format() ) {
      auto view = image->get_view(
        image_view_create_info_t()
          .set_basic(
            vk::ImageViewCreateInfo()
              .setSubresourceRange(
                vk::ImageSubresourceRange()
                  .setAspectMask( vk::ImageAspectFlagBits::eColor )
                  .setBaseMipLevel( 0 )
                  .setLevelCount( image->get_props().get_basic().mipLevels )
                  .setBaseArrayLayer( 0 )
                  .setLayerCount( image->get_props().get_basic().arrayLayers )
              )
              .setViewType( to_image_view_type( image->get_props().get_basic().imageType, image->get_props().get_basic().arrayLayers ) )
              .setFormat( f )
          )
          .rebuild_chain()
      );
      const auto index =
        is_normalized( f ) ?
        req.normalized_index :
        (
          is_scaled( f ) ?
          req.scaled_index :
          req.srgb_index
        );
      texture_state[ index ].set_view( view );
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( index )
              .setDescriptorCount( 1u )
          )
          .add_image( view )
      );
    }
  }
  props.descriptor_set->update(
    std::move( updates )
  );
  execution_pending = true;
}

texture_pool::texture_pool( const texture_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

texture_pool::views texture_pool::allocate(
  const std::string &filename,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( filename, mipmap, attr, max_channels_per_layer );
}

void texture_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}
void texture_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "texture_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->texture_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
    std::optional< request_index_t > write_request_index;
    std::shared_ptr< image_view_t > view;
      if( state->texture_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->texture_state[ i ].write_request_index;
      }
      if( state->texture_state[ i ].view ) {
        temp[ "view" ] = *state->texture_state[ i ].view;
      }
      dest[ "texture_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "normalized_index" ] = w.normalized_index;
    temp[ "scaled_index" ] = w.scaled_index;
    temp[ "srgb_index" ] = w.srgb_index;
    temp[ "filename" ] = w.filename;
    temp[ "mipmap" ] = w.mipmap;
    temp[ "attr" ] = w.attr;
    temp[ "max_channels_per_layer" ] = w.max_channels_per_layer;
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}
void to_json( nlohmann::json &dest, const texture_pool &src ) {
  src.to_json( dest );
}

}

