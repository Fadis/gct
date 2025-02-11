#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/compute.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/command_buffer.hpp>
#include <gct/exception.hpp>
#include <gct/similar_matrix.hpp>
#include <gct/queue.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/generate_random_matrix.hpp>
#include <gct/light_pool.hpp>
namespace gct {

void to_json( nlohmann::json &dest, const punctual_light_parameter &src ) {
  dest = nlohmann::json::object();
  dest[ "local_position" ] = nlohmann::json::array();
  dest[ "local_position" ].push_back( src.local_position.x );
  dest[ "local_position" ].push_back( src.local_position.y );
  dest[ "local_position" ].push_back( src.local_position.z );
  dest[ "local_position" ].push_back( src.local_position.w );
  dest[ "world_position" ] = nlohmann::json::array();
  dest[ "world_position" ].push_back( src.world_position.x );
  dest[ "world_position" ].push_back( src.world_position.y );
  dest[ "world_position" ].push_back( src.world_position.z );
  dest[ "world_position" ].push_back( src.world_position.w );
  dest[ "local_direction" ] = nlohmann::json::array();
  dest[ "local_direction" ].push_back( src.local_direction.x );
  dest[ "local_direction" ].push_back( src.local_direction.y );
  dest[ "local_direction" ].push_back( src.local_direction.z );
  dest[ "local_direction" ].push_back( src.local_direction.w );
  dest[ "world_direction" ] = nlohmann::json::array();
  dest[ "world_direction" ].push_back( src.world_direction.x );
  dest[ "world_direction" ].push_back( src.world_direction.y );
  dest[ "world_direction" ].push_back( src.world_direction.z );
  dest[ "world_direction" ].push_back( src.world_direction.w );
  dest[ "energy" ] = nlohmann::json::array();
  dest[ "energy" ].push_back( src.energy.x );
  dest[ "energy" ].push_back( src.energy.y );
  dest[ "energy" ].push_back( src.energy.z );
  dest[ "energy" ].push_back( src.energy.w );
  dest[ "type" ] = src.type;
  dest[ "light_size" ] = src.light_size;
  dest[ "theta" ] = src.theta;
  dest[ "phi" ] = src.phi;
  dest[ "falloff" ] = src.falloff;
  dest[ "matrix" ] = src.matrix;
}

void from_json( const nlohmann::json &src, punctual_light_parameter &dest ) {
  if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( !src[ "local_position" ].is_array() ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( !src[ "world_position" ].is_array() ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( !src[ "local_direction" ].is_array() ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( !src[ "world_direction" ].is_array() ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( !src[ "energy" ].is_array() ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( src[ "local_position" ].size() != 4u ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( src[ "world_position" ].size() != 4u ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( src[ "local_direction" ].size() != 4u ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( src[ "world_direction" ].size() != 4u ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  if( src[ "energy" ].size() != 4u ) throw incompatible_json( "The JSON is incompatible to punctual_light_parameter", __FILE__, __LINE__ );
  dest.local_position = glm::vec4( float( src[ "local_position" ][ 0 ] ), float( src[ "local_position" ][ 1 ] ), float( src[ "local_position" ][ 2 ] ), float( src[ "local_position" ][ 3 ] ) );
  dest.world_position = glm::vec4( float( src[ "world_position" ][ 0 ] ), float( src[ "world_position" ][ 1 ] ), float( src[ "world_position" ][ 2 ] ), float( src[ "world_position" ][ 3 ] ) );
  dest.local_direction = glm::vec4( float( src[ "local_direction" ][ 0 ] ), float( src[ "local_direction" ][ 1 ] ), float( src[ "local_direction" ][ 2 ] ), float( src[ "local_direction" ][ 3 ] ) );
  dest.world_direction = glm::vec4( float( src[ "world_direction" ][ 0 ] ), float( src[ "world_direction" ][ 1 ] ), float( src[ "world_direction" ][ 2 ] ), float( src[ "world_direction" ][ 3 ] ) );
  dest.energy = glm::vec4( float( src[ "energy" ][ 0 ] ), float( src[ "energy" ][ 1 ] ), float( src[ "energy" ][ 2 ] ), float( src[ "energy" ][ 3 ] ) );
  dest.type = src[ "type" ];
  dest.light_size = src[ "light_size" ];
  dest.theta = src[ "theta" ];
  dest.phi = src[ "phi" ];
  dest.falloff = src[ "falloff" ];
  dest.matrix = src[ "matrix" ];
}

light_pool::light_index_t light_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= light_state.size() ) {
    light_state.resize( index + 1u );
  }
  return index; 
}

void light_pool::state_type::release_index( light_pool::light_index_t index ) {
  if( index >= light_state.size() || !light_state[ index ].valid ) {
    return;
  }
  light_state[ index ].valid = false;
  index_allocator.release( index );
}

light_pool::light_descriptor light_pool::state_type::allocate( const matrix_pool::matrix_descriptor &matrix, const light_type &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "light_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  auto write_requests = write_request_buffer->map< write_request >();
  auto update_requests = update_request_buffer->map< update_request >();
  auto staging = staging_light->map< light_type >();

  const light_index_t index = allocate_index();

  const light_index_t staging_index = staging_index_allocator.allocate();

  const request_index_t write_request_index = write_request_index_allocator.allocate();
  const request_index_t update_request_index = update_request_index_allocator.allocate();

  write_requests[ write_request_index ] =
    write_request()
      .set_staging( staging_index )
      .set_destination( index );

  auto value_ = value;
  value_.set_matrix( *matrix );
  staging[ staging_index ] = value_;

  light_state[ index ] =
    light_state_type()
      .set_valid( true )
      .set_matrix( matrix )
      .set_staging_index( staging_index )
      .set_write_request_index( write_request_index );

  update_requests[ update_request_index ] =
    update_request()
      .set_index( index );
  light_descriptor desc(
    new light_index_t( index ),
    [self=shared_from_this()]( const light_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );

  used_on_gpu.push_back( desc );
  light_state[ index ].set_self( desc.get_weak() );
  
  if( glm::vec3( value.energy ) != glm::vec3( 0.f, 0.f, 0.f ) ) {
    host_active_light.insert( *desc );
    active_light_modified = true;
  }
  return desc;
}

void light_pool::state_type::release( light_index_t index ) {
  matrix_pool::matrix_descriptor matrix;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( light_state.size() <= index || !light_state[ index ].valid ) {
      return;
    }
    release_index( index );
    matrix = light_state[ index ].matrix;
    light_state[ index ] = light_state_type();
    host_active_light.erase( index );
    active_light_modified = true;
  }
}

void light_pool::state_type::touch( const light_descriptor &desc ) {
  if( execution_pending ) {
    throw exception::runtime_error( "light_pool::state_type::touch : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( light_state.size() <= *desc || !light_state[ *desc ].valid ) {
    return;
  }
  if( light_state[ *desc ].update_request_index ) {
    return;
  }
  update_request_buffer->map< update_request >()[ *desc ] =
    update_request()
      .set_index( *desc );
  used_on_gpu.push_back( desc );
}

void light_pool::state_type::touch( light_index_t index ) {
  if( light_state.size() <= index || !light_state[ index ].valid ) {
    return;
  }
  if( light_state[ index ].update_request_index ) {
    return;
  }
  const auto &s = light_state[ index ];
  auto desc = light_descriptor( s.self );
  touch( desc );
}

void light_pool::state_type::set( const light_descriptor &desc, const light_type &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "light_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( light_state.size() <= *desc || !light_state[ *desc ].valid ) {
    return;
  }
  auto staging = staging_light->map< light_type >();
  auto &s = light_state[ *desc ];
  auto value_ = value;
  value_.set_matrix( *s.matrix );
  if( s.staging_index && s.write_request_index ) {
    staging[ *s.staging_index ] = value_;
  }
  else if( s.staging_index ) {
    auto write_requests = write_request_buffer->map< write_request >();
    staging[ *s.staging_index ] = value_;
    const request_index_t write_request_index = write_request_index_allocator.allocate();
    write_requests[ write_request_index ] =
      write_request()
        .set_staging( *s.staging_index )
        .set_destination( *desc );
    s.set_write_request_index( write_request_index );
  }
  else {
    auto write_requests = write_request_buffer->map< write_request >();
    const light_index_t staging_index = staging_index_allocator.allocate();
    staging[ staging_index ] = value_;
    const request_index_t write_request_index = write_request_index_allocator.allocate();
    write_requests[ write_request_index ] =
      write_request()
        .set_staging( staging_index )
        .set_destination( *desc );
    s.set_write_request_index( write_request_index );
    s.set_staging_index( staging_index );
    used_on_gpu.push_back( desc );
  }
  if( !s.update_request_index ) {
    auto update_requests = update_request_buffer->map< update_request >();
    const request_index_t update_request_index = update_request_index_allocator.allocate();
    update_requests[ update_request_index ] =
      update_request()
        .set_index( *desc );
    s.set_update_request_index( update_request_index );
  }
  if( glm::vec3( value.energy ) == glm::vec3( 0.f, 0.f, 0.f ) ) {
    if( host_active_light.find( *desc ) != host_active_light.end() ) {
      host_active_light.erase( *desc );
      active_light_modified = true;
    }
  }
  else {
    if( host_active_light.find( *desc ) == host_active_light.end() ) {
      host_active_light.insert( *desc );
      active_light_modified = true;
    }
  }
}

void light_pool::state_type::get( const light_descriptor &desc, const std::function< void( vk::Result, const light_type& ) > &cb ) {
  if( execution_pending ) {
    throw exception::runtime_error( "light_pool::state_type::get : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( light_state.size() <= *desc || !light_state[ *desc ].valid ) {
    return;
  }
  auto read_requests = read_request_buffer->map< read_request >();
  auto &s = light_state[ *desc ];
  if( s.staging_index && s.read_request_index ) {
    cbs.insert( std::make_pair( *desc, cb ) );
  }
  else if( s.staging_index ) {
    cbs.insert( std::make_pair( *desc, cb ) );
    const request_index_t read_request_index = read_request_index_allocator.allocate();
    read_requests[ read_request_index ] =
      read_request()
        .set_source( *desc )
        .set_staging( *s.staging_index );
    s.set_read_request_index( read_request_index );
  }
  else {
    cbs.insert( std::make_pair( *desc, cb ) );
    const light_index_t staging_index = staging_index_allocator.allocate();
    const request_index_t read_request_index = read_request_index_allocator.allocate();
    read_requests[ read_request_index ] =
      read_request()
        .set_source( *desc )
        .set_staging( staging_index );
    s.set_read_request_index( read_request_index );
    s.set_staging_index( staging_index );
    used_on_gpu.push_back( desc );
  }
}

std::uint32_t light_pool::state_type::get_active_light_count() const {
  return host_active_light.size();
}

light_pool::state_type::state_type( const light_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_light_count ) ),
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_light_count ) ),
  write_request_index_allocator( linear_allocator_create_info().set_max( ci.max_light_count ) ),
  read_request_index_allocator( linear_allocator_create_info().set_max( ci.max_light_count ) ),
  update_request_index_allocator( linear_allocator_create_info().set_max( ci.max_light_count ) )
{
  light = props.allocator_set.allocator->create_buffer( sizeof( light_type ) * props.max_light_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY );
  active_light = props.allocator_set.allocator->create_mappable_buffer( sizeof( std::uint32_t ) * props.max_light_count, vk::BufferUsageFlagBits::eStorageBuffer );
  staging_light = props.allocator_set.allocator->create_buffer( sizeof( light_type ) * props.max_light_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );

  write_request_buffer = props.allocator_set.allocator->create_buffer( sizeof( write_request ) * props.max_light_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  read_request_buffer = props.allocator_set.allocator->create_buffer( sizeof( read_request ) * props.max_light_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  update_request_buffer = props.allocator_set.allocator->create_buffer( sizeof( update_request ) * props.max_light_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );

  write.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.write_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.light_buffer_name, light } )
      .add_resource( { props.staging_light_buffer_name, staging_light } )
      .add_resource( { props.write_request_buffer_name, write_request_buffer } )
  ) );
  read.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.read_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.light_buffer_name, light } )
      .add_resource( { props.staging_light_buffer_name, staging_light } )
      .add_resource( { props.read_request_buffer_name, read_request_buffer } )
  ) );
  update.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.update_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.light_buffer_name, light } )
      .add_resource( { props.update_request_buffer_name, update_request_buffer } )
      .add_resource( { props.matrix_buffer_name, props.matrix_pool } )
  ) );
}

void light_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  if( active_light_modified ) {
    {
      auto dest = active_light->map< std::uint32_t >();
      std::copy(
        host_active_light.begin(),
        host_active_light.end(),
        dest.begin()
      );
    }
    rec.sync_to_device( active_light );
    rec.barrier( { active_light->get_buffer() }, {} );
  }

  if( write_request_index_allocator.get_tail() ) {
    request_range range =
      request_range()
        .set_count( write_request_index_allocator.get_tail() );
    rec->pushConstants(
        **write->get_pipeline()->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( request_range ),
        reinterpret_cast< void* >( &range )
    );
    (*write)( rec, 0u, write_request_index_allocator.get_tail(), 1u, 1u );
  }
  rec.barrier( { light }, {} );
  if( update_request_index_allocator.get_tail() ) {
    request_range range =
      request_range()
        .set_count( update_request_index_allocator.get_tail() );
    rec->pushConstants(
        **update->get_pipeline()->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( request_range ),
        reinterpret_cast< void* >( &range )
    );
    (*update)( rec, 0u, update_request_index_allocator.get_tail(), 1u, 1u );
  }
  rec.barrier( { light }, {} );
  if( read_request_index_allocator.get_tail() ) {
    request_range range =
      request_range()
        .set_count( read_request_index_allocator.get_tail() );
    rec->pushConstants(
        **read->get_pipeline()->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( request_range ),
        reinterpret_cast< void* >( &range )
    );
    (*read)( rec, 0u, read_request_index_allocator.get_tail(), 1u, 1u );
  }
  rec.compute_barrier( { light }, {} );
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< std::function< void() > > cbs;
      std::vector< light_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        auto staging = self->staging_light->map< light_type >();
        for( const auto &desc: self->used_on_gpu ) {
          if( self->light_state.size() > *desc && self->light_state[ *desc ].valid ) {
            auto &s = self->light_state[ *desc ];
            if( s.read_request_index && s.staging_index ) {
              const auto corresponding = self->cbs.equal_range( *desc );
              if( corresponding.first != corresponding.second ) {
                if( result == vk::Result::eSuccess ) {
                  auto light = staging[ *s.staging_index ];
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result,light]() { cb( result, light ); } );
                  }
                }
                else {
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result]() { cb( result, light_type() ); } );
                  }
                }
              }
            }
            s.write_request_index = std::nullopt;
            s.read_request_index = std::nullopt;
            s.update_request_index = std::nullopt;
            s.staging_index = std::nullopt;
          }
        }
        self->write_request_index_allocator.reset();
        self->read_request_index_allocator.reset();
        self->update_request_index_allocator.reset();
        self->staging_index_allocator.reset();
        self->cbs.clear();
        used_on_gpu = std::move( self->used_on_gpu );
        self->used_on_gpu.clear();
        self->execution_pending = false;
      }
      for( auto &cb: cbs ) {
        cb();
      }
    }
  );
  execution_pending = true;
}

light_pool::light_pool( const light_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

light_pool::light_descriptor light_pool::allocate( const matrix_pool::matrix_descriptor &matrix, const light_type &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( matrix, value );
}
void light_pool::touch( const light_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->touch( desc );
}
void light_pool::set( const light_descriptor &desc, const light_type &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->set( desc, value );
}
void light_pool::get( const light_descriptor &desc, const std::function< void( vk::Result, const light_type& ) > &cb ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->get( desc, cb );
}
void light_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}
std::shared_ptr< buffer_t > light_pool::get_active_light_buffer() const {
  return state->active_light->get_buffer();
}
void light_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "light_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->light_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->light_state[ i ].staging_index ) {
        temp[ "staging_index" ] = *state->light_state[ i ].staging_index;
      }
      if( state->light_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->light_state[ i ].write_request_index;
      }
      if( state->light_state[ i ].read_request_index ) {
        temp[ "read_request_index" ] = *state->light_state[ i ].read_request_index;
      }
      if( state->light_state[ i ].update_request_index ) {
        temp[ "update_request_index" ] = *state->light_state[ i ].update_request_index;
      }
      if( state->light_state[ i ].matrix ) {
        temp[ "matrix" ] = *state->light_state[ i ].matrix;
      }
      {
        auto d = state->light_state[ i ].self.lock();
        if( d ) {
          temp[ "self" ] = *d;
        }
      }
      dest[ "light_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "index_allocator" ] = state->index_allocator;
  dest[ "staging_light" ] = nlohmann::json::array();
  {
    auto m = state->staging_light->map< light_type >();
    for( std::uint32_t i = 0u; i != state->staging_index_allocator.get_tail(); ++i ) {
      dest[ "staging_light" ].push_back( m[ i ] );
    }
  }
  dest[ "light" ] = *state->light;
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  {
    auto m = state->write_request_buffer->map< write_request >();
    for( std::uint32_t i = 0u; i != state->write_request_index_allocator.get_tail(); ++i ) {
      auto e = m[ i ];
      auto temp = nlohmann::json::object();
      temp[ "staging" ] = e.staging;
      temp[ "destination" ] = e.destination;
      dest[ "write_request_buffer" ].push_back( temp );
    }
  }
  dest[ "read_request_buffer" ] = nlohmann::json::array();
  {
    auto m = state->read_request_buffer->map< read_request >();
    for( std::uint32_t i = 0u; i != state->read_request_index_allocator.get_tail(); ++i ) {
      auto e = m[ i ];
      auto temp = nlohmann::json::object();
      temp[ "source" ] = e.source;
      temp[ "staging" ] = e.staging;
      dest[ "read_request_buffer" ].push_back( temp );
    }
  }
  dest[ "update_request_buffer" ] = nlohmann::json::array();
  {
    auto m = state->update_request_buffer->map< update_request >();
    for( std::uint32_t i = 0u; i != state->update_request_index_allocator.get_tail(); ++i ) {
      auto e = m[ i ];
      auto temp = nlohmann::json::object();
      temp[ "index" ] = e.index;
      dest[ "update_request_buffer" ].push_back( temp );
    }
  }
  dest[ "staging_index_allocator" ] = state->staging_index_allocator;
  dest[ "write_request_index_allocator" ] = state->write_request_index_allocator;
  dest[ "read_request_index_allocator" ] = state->read_request_index_allocator;
  dest[ "update_request_index_allocator" ] = state->update_request_index_allocator;
  dest[ "used_on_gpu" ] = nlohmann::json::array();
  for( const auto &e: state->used_on_gpu ) {
    dest[ "used_on_gpu" ].push_back( *e );
  }
  dest[ "active_light" ] = nlohmann::json::array();
  for( auto i: state->host_active_light ) {
    dest[ "active_light" ].push_back( i );
  }
  dest[ "write" ] = *state->write;
  dest[ "read" ] = *state->read;
  dest[ "update" ] = *state->update;
  dest[ "execution_pending" ] = state->execution_pending;
}
void to_json( nlohmann::json &dest, const light_pool &src ) {
  src.to_json( dest );
}

std::pair<
  std::vector< light_pool::light_descriptor >,
  std::vector< punctual_light_parameter >
> generate_random_light(
  light_pool &pool,
  const matrix_pool::matrix_descriptor &matrix,
  const aabb3 &range,
  const aabb3 &energy,
  float min_energy,
  float max_energy,
  std::uint32_t count
) {
  std::uniform_real_distribution<float> x_dist( range.min.x, range.max.x );
  std::uniform_real_distribution<float> y_dist( range.min.y, range.max.y );
  std::uniform_real_distribution<float> z_dist( range.min.z, range.max.z );
  std::uniform_real_distribution<float> r_dist( energy.min.x, energy.max.x );
  std::uniform_real_distribution<float> g_dist( energy.min.y, energy.max.y );
  std::uniform_real_distribution<float> b_dist( energy.min.z, energy.max.z );
  std::uniform_real_distribution<float> e_dist( min_energy, max_energy );
  std::mt19937 engine( std::random_device{}() );
  std::vector< light_pool::light_descriptor > lights;
  std::vector< punctual_light_parameter > params;
  for( unsigned int i = 0u; i != count; ++i ) {
    const auto param = 
      punctual_light_parameter()
        .set_type( punctual_light_type::point )
        .set_light_size( 5.f )
        .set_local_position( glm::vec4(
          x_dist( engine ),
          -y_dist( engine ),
          z_dist( engine ),
          1.f
        ) )
        .set_energy( glm::normalize( glm::vec4(
          r_dist( engine ),
          g_dist( engine ),
          b_dist( engine ),
          1.f
        ) ) * e_dist( engine ) );
    lights.push_back(
      pool.allocate(
        matrix,
        param
      )
    );
    params.push_back( param );
  }
  return std::make_pair( lights, params );
}
std::vector< light_pool::light_descriptor >
restore_light(
  light_pool &pool,
  const matrix_pool::matrix_descriptor &matrix,
  const std::vector< punctual_light_parameter > &params
) {
  std::vector< light_pool::light_descriptor > lights;
  for( const auto &p: params ) {
    lights.push_back(
      pool.allocate(
        matrix,
        p
      )
    );
  }
  return lights;
}

}

