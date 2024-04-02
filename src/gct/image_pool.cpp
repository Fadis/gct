#include "gct/exception.hpp"
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/numeric_types.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/buffer.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/create_image_from_file.hpp>
#include <gct/image_pool_create_info.hpp>
#include <gct/image_pool.hpp>

namespace gct {

image_pool::image_index_t image_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= image_state.size() ) {
    image_state.resize( index + 1u );
  }
  return index; 
}

void image_pool::state_type::release_index( image_pool::image_index_t index ) {
  if( index >= image_state.size() || !image_state[ index ].valid ) {
    return;
  }
  image_state[ index ].valid = false;
  index_allocator.release( index );
}

image_pool::views image_pool::state_type::allocate() {

  const image_index_t normalized_index = allocate_index();
  const image_index_t srgb_index = allocate_index();

  auto b = props.allocator->create_buffer(
    16 * 16 * 4,
    vk::BufferUsageFlagBits::eTransferSrc,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );
  
  {
    auto mapped = b->map< std::uint8_t >();
    std::fill( mapped.begin(), mapped.end(), 255u );
  }

  numeric_type_t nt;
  nt.composite = numeric_composite_type_t::vector;
  nt.rows = 4u;
  nt.component = numeric_component_type_t::int_;
  nt.depth = 8u;
  nt.sign = false;
  vk::Format primary_format = vk::Format::eUndefined;
  std::vector< vk::Format > secondary_formats;
  const auto attr = integer_attribute_t::normalized;
  constexpr static std::array< integer_attribute_t, 2u > attrs{
    integer_attribute_t::normalized,
    integer_attribute_t::srgb
  };
  for( const auto a: attrs ) {
    nt.attr = a;
    const auto &formats = get_compatible_format( nt );
    if( formats.empty() ) {
      if( a == attr ) throw -1;
    }
    else {
      secondary_formats.push_back( formats[ 0 ] );
      if( a == attr ) {
        primary_format = formats[ 0 ];
      }
    }
  }

  std::shared_ptr< image_t > i = props.allocator->create_image(
    image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( primary_format )
          .setExtent( { 16, 16, 1 } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage( vk::ImageUsageFlagBits::eTransferSrc | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eSampled )
          .setSharingMode( vk::SharingMode::eExclusive )
          .setQueueFamilyIndexCount( 0 )
          .setPQueueFamilyIndices( nullptr )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      )
      .add_format( secondary_formats.begin(), secondary_formats.end() ),
      VMA_MEMORY_USAGE_GPU_ONLY
  );

  std::shared_ptr< image_view_t > normalized;
  std::shared_ptr< image_view_t > srgb;

  for( auto f: i->get_props().get_format() ) {
    auto view = i->get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( i->get_props().get_basic().mipLevels )
                .setBaseArrayLayer( 0 )
                .setLayerCount( i->get_props().get_basic().arrayLayers )
            )
            .setViewType( to_image_view_type( i->get_props().get_basic().imageType, i->get_props().get_basic().arrayLayers ) )
            .setFormat( f )
        )
        .rebuild_chain()
    );
    if( is_normalized( f ) ) {
      normalized = view;
    }
    else if( is_srgb( f ) ) {
      srgb = view;
    }
  }

  write_request_list.push_back(
    write_request()
      .set_index( normalized_index )
      .set_mipmap( false )
      .set_staging_buffer( b )
      .set_final_image( normalized )
  );
  image_state[ normalized_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( normalized )
      .set_write_request_index( write_request_list.size() - 1u );

  image_descriptor normalized_desc(
    new image_index_t( normalized_index ),
    [self=shared_from_this()]( const image_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( normalized_desc );

  write_request_list.push_back(
    write_request()
      .set_index( srgb_index )
      .set_mipmap( false )
      .set_staging_buffer( b )
      .set_final_image( srgb )
  );
  image_state[ srgb_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( srgb )
      .set_write_request_index( write_request_list.size() - 1u );

  image_descriptor srgb_desc(
    new image_index_t( srgb_index ),
    [self=shared_from_this()]( const image_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( srgb_desc );

  return views()
    .set_normalized( normalized_desc )
    .set_srgb( srgb_desc );
}

image_pool::views image_pool::state_type::allocate(
  const image_load_info &ci
) {
  if( execution_pending ) {
    throw exception::runtime_error( "image_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  const image_index_t normalized_index = allocate_index();
  const image_index_t srgb_index = allocate_index();

  auto &device = get_device( *props.allocator );

  const auto [b,i] = create_image_from_file(
    props.allocator,
    ci.filename,
    ci.usage,
    ci.mipmap,
    ci.attr,
    ci.max_channels_per_layer
  );

  std::shared_ptr< image_view_t > normalized;
  std::shared_ptr< image_view_t > srgb;

  for( auto f: i->get_props().get_format() ) {
    auto view = i->get_view(
      image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( i->get_props().get_basic().mipLevels )
                .setBaseArrayLayer( 0 )
                .setLayerCount( i->get_props().get_basic().arrayLayers )
            )
            .setViewType( to_image_view_type( i->get_props().get_basic().imageType, i->get_props().get_basic().arrayLayers ) )
            .setFormat( f )
        )
        .rebuild_chain()
    );
    if( is_normalized( f ) ) {
      normalized = view;
    }
    else if( is_srgb( f ) ) {
      srgb = view;
    }
  }

  write_request_list.push_back(
    write_request()
      .set_index( normalized_index )
      .set_mipmap( ci.mipmap )
      .set_staging_buffer( b )
      .set_final_image( normalized )
  );
  image_state[ normalized_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( normalized )
      .set_write_request_index( write_request_list.size() - 1u );

  image_descriptor normalized_desc(
    new image_index_t( normalized_index ),
    [self=shared_from_this()]( const image_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( normalized_desc );

  write_request_list.push_back(
    write_request()
      .set_index( srgb_index )
      .set_mipmap( ci.mipmap )
      .set_staging_buffer( b )
      .set_final_image( srgb )
  );
  image_state[ srgb_index ] =
    image_state_type()
      .set_valid( true )
      .set_image( srgb )
      .set_write_request_index( write_request_list.size() - 1u );

  image_descriptor srgb_desc(
    new image_index_t( srgb_index ),
    [self=shared_from_this()]( const image_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( srgb_desc );

  return views()
    .set_normalized( normalized_desc )
    .set_srgb( srgb_desc );
}

void image_pool::state_type::release( image_index_t index ) {
  image_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( image_state.size() <= index || !image_state[ index ].valid ) {
      return;
    }
    removed = image_state[ index ];
    image_state[ index ] = image_state_type();
    release_index( index );
    if( props.descriptor_set ) {
      const auto target = (*props.descriptor_set)[ props.descriptor_name ];
      /*std::vector< write_descriptor_set_t > updates;
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( index )
              .setDescriptorCount( 1u )
          )
      );
      props.descriptor_set->update(
        std::move( updates )
      );*/
    }
  }
}

void image_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  for( const auto &req: write_request_list ) {
    rec.buffer_to_image( req.mipmap, req.staging_buffer, req.final_image->get_factory() );
  }
  for( const auto &req: write_request_list ) {
    rec.convert_image( req.final_image->get_factory(), props.layout );
  }
  if( props.descriptor_set ) {
    std::vector< write_descriptor_set_t > updates;
    const auto target = (*props.descriptor_set)[ props.descriptor_name ];
    for( const auto &req: write_request_list ) {
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( req.index )
              .setDescriptorCount( 1u )
          )
          .add_image( req.final_image, props.layout )
      );
    }
    props.descriptor_set->update(
      std::move( updates )
    );
  }
  for( const auto &req: dump_request_list ) {
    rec.dump_image(
      props.allocator,
      req.first,
      req.second.filename,
      req.second.mipmap,
      req.second.layer,
      req.second.depth
    );
  }
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< image_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        for( const auto &desc: self->used_on_gpu ) {
          if( self->image_state.size() > *desc && self->image_state[ *desc ].valid ) {
            auto &s = self->image_state[ *desc ];
            s.write_request_index = std::nullopt;
          }
        }
        self->write_request_list.clear();
        used_on_gpu = std::move( self->used_on_gpu );
        self->used_on_gpu.clear();
        self->execution_pending = false;
      }
    }
  );
  execution_pending = true;
}

std::shared_ptr< image_view_t > image_pool::state_type::get(
  const image_descriptor &desc
) const {
  if( image_state.size() <= *desc || !image_state[ *desc ].valid ) {
    throw exception::invalid_argument( "image_pool::get : No such image" );
  }
  return image_state[ *desc ].image;
}

void image_pool::state_type::dump( const image_descriptor &desc, const image_dump_info &ci ) {
  if( image_state.size() <= *desc || !image_state[ *desc ].valid ) {
    throw exception::invalid_argument( "image_pool::dump : No such image" );
  }
  dump_request_list.push_back( std::make_pair( image_state[ *desc ].image->get_factory(), ci ) );
}

image_pool::state_type::state_type( const image_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_image_count ) ) {
}

image_pool::image_pool( const image_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

image_pool::views image_pool::allocate(
  const image_load_info &ci
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( ci );
}

image_pool::views image_pool::allocate() {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate();
}

std::shared_ptr< image_view_t > image_pool::get(
  const image_descriptor &desc
) const {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get( desc );
}

void image_pool::dump( const image_descriptor &desc, const image_dump_info &ci ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->dump( desc, ci );
}

void image_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}

void image_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "sampler_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->image_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->image_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->image_state[ i ].write_request_index;
      }
      if( state->image_state[ i ].image ) {
        temp[ "image" ] = *state->image_state[ i ].image;
      }
      dest[ "sampler_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "index" ] = w.index;
    if( w.staging_buffer ) {
      temp[ "staging_buffer" ] = *w.staging_buffer;
    }
    if( w.final_image ) {
      temp[ "final_image" ] = *w.final_image;
    }
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}


void to_json( nlohmann::json &dest, const image_pool &src ) {
  src.to_json( dest );
}
}
