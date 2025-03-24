#include <nlohmann/json.hpp>
#include <vulkan2json/DescriptorImageInfo.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/compute.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/command_buffer.hpp>
#include <gct/exception.hpp>
#include <gct/texture_pool.hpp>
#include <gct/sampler_pool.hpp>

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
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::views &iid
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  const auto s = props.sampler->get( sid );
  if( !s ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : sampler not found", __FILE__, __LINE__ );
  }

  const texture_index_t normalized_index = iid.normalized ? allocate_index() : texture_index_t( 0u );
  const texture_index_t srgb_index = iid.srgb ? allocate_index() : texture_index_t( 0u );
  const texture_index_t linear_index = iid.linear ? allocate_index() : texture_index_t( 0u );

  texture_descriptor normalized_desc;
  texture_descriptor srgb_desc;
  texture_descriptor linear_desc;
  if( iid.normalized ) {
    const auto normalized = props.image->get( iid.normalized );
    write_request_list.push_back(
      write_request()
        .set_index( normalized_index )
        .set_sampler( s )
        .set_view( normalized )
    );
    texture_state[ normalized_index ] =
      texture_state_type()
        .set_valid( true )
        .set_write_request_index( write_request_list.size() - 1u )
        .set_sampler( s )
        .set_view( normalized );
    normalized_desc = texture_descriptor(
      new texture_index_t( normalized_index ),
      [self=shared_from_this()]( const texture_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( normalized_desc );
  }

  if( iid.srgb ) {
    const auto srgb = props.image->get( iid.srgb ); 
    write_request_list.push_back(
      write_request()
        .set_index( srgb_index )
        .set_sampler( s )
        .set_view( srgb )
    );
    texture_state[ srgb_index ] =
      texture_state_type()
        .set_valid( true )
        .set_write_request_index( write_request_list.size() - 1u )
        .set_sampler( s )
        .set_view( srgb );
 
    srgb_desc = texture_descriptor(
      new texture_index_t( srgb_index ),
      [self=shared_from_this()]( const texture_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( srgb_desc );
  }
  
  if( iid.linear ) {
    const auto linear = props.image->get( iid.linear );
    write_request_list.push_back(
      write_request()
        .set_index( linear_index )
        .set_sampler( s )
        .set_view( linear )
    );
    texture_state[ linear_index ] =
      texture_state_type()
        .set_valid( true )
        .set_write_request_index( write_request_list.size() - 1u )
        .set_sampler( s )
        .set_view( linear );
 
    linear_desc = texture_descriptor(
      new texture_index_t( linear_index ),
      [self=shared_from_this()]( const texture_index_t *p ) {
        if( p ) {
          self->release( *p );
          delete p;
        }
      }
    );
    used_on_gpu.push_back( linear_desc );
  }

  return views()
    .set_normalized( normalized_desc )
    .set_srgb( srgb_desc )
    .set_linear( linear_desc );
}

texture_pool::texture_descriptor texture_pool::state_type::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::image_descriptor &iid
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  const texture_index_t index = allocate_index();
    
  const auto s = props.sampler->get( sid );
  const auto linear = props.image->get( iid );

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_sampler( s )
      .set_view( linear )
  );
  texture_state[ index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u )
      .set_sampler( s )
      .set_view( linear );
 
  auto desc = texture_descriptor(
    new texture_index_t( index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );

  return desc;
}

texture_pool::texture_descriptor texture_pool::state_type::allocate(
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &linear
) {
  if( execution_pending ) {
    throw exception::runtime_error( "texture_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  const texture_index_t index = allocate_index();

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_sampler( s )
      .set_view( linear )
  );
  texture_state[ index ] =
    texture_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u )
      .set_sampler( s )
      .set_view( linear );
 
  auto desc = texture_descriptor(
    new texture_index_t( index ),
    [self=shared_from_this()]( const texture_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );

  return desc;
}


std::pair< std::shared_ptr< image_view_t >, std::shared_ptr< sampler_t > > texture_pool::state_type::get(
  const texture_descriptor &desc
) const {
  if( texture_state.size() <= *desc || !texture_state[ *desc ].valid ) {
    throw exception::runtime_error( "texture_pool::state_type::get : no such texture", __FILE__, __LINE__ );
  }
  return std::make_pair(
    texture_state[ *desc ].view,
    texture_state[ *desc ].sampler
  );
}

void texture_pool::state_type::release( texture_index_t index ) {
  texture_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( texture_state.size() <= index || !texture_state[ index ].valid ) {
      return;
    }
    removed = std::move( texture_state[ index ] );
    texture_state[ index ] = texture_state_type();
    release_index( index );
    /*if( props.descriptor_set ) {
      const auto target = (*props.descriptor_set)[ props.descriptor_name ];
      std::vector< write_descriptor_set_t > updates;
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
      );
    }*/
  }
}

texture_pool::state_type::state_type( const texture_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_texture_count ) ) {
  allocate_index();
}

void texture_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  std::vector< texture_descriptor > used_on_gpu_;
  {
    std::lock_guard< std::mutex > lock( guard );
    std::vector< write_descriptor_set_t > updates;
    const auto target = (*props.descriptor_set)[ props.descriptor_name ];
    for( const auto &req: write_request_list ) {
      if( props.descriptor_set ) {
        auto u = write_descriptor_set_t()
            .set_basic(
              vk::WriteDescriptorSet( target )
                .setDstArrayElement( req.index )
                .setDescriptorCount( 1u )
            )
            .add_image( req.sampler, req.view, vk::ImageLayout::eShaderReadOnlyOptimal )
            .set_index( req.index );
        updates.push_back( u );
        props.descriptor_set->update(
          std::move( updates )
        );
        updates.clear();
      }
    }
    if( props.descriptor_set ) {
      //props.descriptor_set->update(
      //  std::move( updates )
      //);
    }
    for( const auto &desc: used_on_gpu ) {
      if( texture_state.size() > *desc && texture_state[ *desc ].valid ) {
        auto &s = texture_state[ *desc ];
        s.write_request_index = std::nullopt;
      }
    }
    write_request_list.clear();
    used_on_gpu_ = std::move( used_on_gpu );
    used_on_gpu.clear();
  }
}

texture_pool::texture_pool( const texture_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

texture_pool::views texture_pool::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::views &iid
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( sid, iid );
}

texture_pool::texture_descriptor texture_pool::allocate(
  const sampler_pool::sampler_descriptor &sid,
  const image_pool::image_descriptor &iid
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( sid, iid );
}


texture_pool::texture_descriptor texture_pool::allocate(
  const std::shared_ptr< sampler_t > &s,
  const std::shared_ptr< image_view_t > &linear
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( s, linear );
}

std::pair< std::shared_ptr< image_view_t >, std::shared_ptr< sampler_t > > texture_pool::get(
  const texture_descriptor &desc
) const {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get( desc );
}

void texture_pool::operator()( command_buffer_recorder_t &rec ) {
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
      if( state->texture_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->texture_state[ i ].write_request_index;
      }
      if( state->texture_state[ i ].view ) {
        temp[ "view" ] = *state->texture_state[ i ].view;
      }
      if( state->texture_state[ i ].sampler ) {
        temp[ "sampler" ] = *state->texture_state[ i ].sampler;
      }
      dest[ "texture_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "index" ] = w.index;
    if( w.view ) {
      temp[ "view" ] = *w.view;
    }
    if( w.sampler ) {
      temp[ "sampler" ] = *w.sampler;
    }
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}
void to_json( nlohmann::json &dest, const texture_pool &src ) {
  src.to_json( dest );
}

}

