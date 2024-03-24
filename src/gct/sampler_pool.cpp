#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/descriptor_pool_create_info.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout_create_info.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/sampler.hpp>
#include <gct/allocator.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/get_device.hpp>
#include <gct/sampler_pool_create_info.hpp>
#include <gct/sampler_pool.hpp>

namespace gct {

sampler_pool::sampler_index_t sampler_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= sampler_state.size() ) {
    sampler_state.resize( index + 1u );
  }
  return index; 
}

void sampler_pool::state_type::release_index( sampler_pool::sampler_index_t index ) {
  if( index >= sampler_state.size() || !sampler_state[ index ].valid ) {
    return;
  }
  sampler_state[ index ].valid = false;
  index_allocator.release( index );
}

sampler_pool::sampler_descriptor sampler_pool::state_type::allocate(
  const sampler_create_info_t &ci
) {
  if( execution_pending ) {
    throw exception::runtime_error( "sampler_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  const sampler_index_t index = allocate_index();

  auto &device = get_device( *props.allocator );

  const auto s = device.get_sampler( ci );

  write_request_list.push_back(
    write_request()
      .set_sampler( s )
      .set_index( index )
  );
  sampler_state[ index ] =
    sampler_state_type()
      .set_valid( true )
      .set_write_request_index( write_request_list.size() - 1u );

  sampler_descriptor desc(
    new sampler_index_t( index ),
    [self=shared_from_this()]( const sampler_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );

  return desc;
}

void sampler_pool::state_type::release( sampler_index_t index ) {
  sampler_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( sampler_state.size() <= index || !sampler_state[ index ].valid ) {
      return;
    }
    removed = sampler_state[ index ];
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
        .add_sampler( null_sampler )
    );
    props.descriptor_set->update(
      std::move( updates )
    );
  }
}

void sampler_pool::state_type::flush() {
  std::vector< sampler_descriptor > used_on_gpu_;
  {
    std::lock_guard< std::mutex > lock( guard );
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
          .add_sampler( req.sampler )
      );
    }
    props.descriptor_set->update(
      std::move( updates )
    );
    for( const auto &desc: used_on_gpu ) {
      if( sampler_state.size() > *desc && sampler_state[ *desc ].valid ) {
        auto &s = sampler_state[ *desc ];
        s.write_request_index = std::nullopt;
      }
    }
    write_request_list.clear();
    used_on_gpu_ = std::move( used_on_gpu );
    used_on_gpu.clear();
  }
}

sampler_pool::state_type::state_type( const sampler_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_sampler_count ) ) {
}

sampler_pool::sampler_pool( const sampler_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

sampler_pool::sampler_descriptor sampler_pool::allocate(
  const sampler_create_info_t &ci
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( ci );
}

void sampler_pool::operator()() {
  state->flush();
}

void sampler_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "sampler_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->sampler_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->sampler_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->sampler_state[ i ].write_request_index;
      }
      if( state->sampler_state[ i ].sampler ) {
        temp[ "sampler" ] = *state->sampler_state[ i ].sampler;
      }
      dest[ "sampler_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "index" ] = w.index;
    if( w.sampler ) {
      temp[ "sampler" ] = *w.sampler;
    }
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}


void to_json( nlohmann::json &dest, const sampler_pool &src ) {
  src.to_json( dest );
}
}
