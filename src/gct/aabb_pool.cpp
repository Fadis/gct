#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
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
#include <gct/aabb_pool.hpp>
namespace gct {

aabb_pool::aabb_index_t aabb_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= aabb_state.size() ) {
    aabb_state.resize( index + 1u );
  }
  return index; 
}

void aabb_pool::state_type::release_index( aabb_pool::aabb_index_t index ) {
  if( index >= aabb_state.size() || !aabb_state[ index ].valid ) {
    return;
  }
  aabb_state[ index ].valid = false;
  index_allocator.release( index );
}

aabb_pool::aabb_descriptor aabb_pool::state_type::allocate( const aabb_type &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "aabb_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  auto write_requests = write_request_buffer->map< write_request >();
  auto staging = staging_aabb->map< aabb_type >();

  const aabb_index_t index = allocate_index();

  const aabb_index_t staging_index = staging_index_allocator.allocate();

  const request_index_t write_request_index = write_request_index_allocator.allocate();

  write_requests[ write_request_index ] =
    write_request()
      .set_staging( staging_index )
      .set_destination( index );

  staging[ staging_index ] = value;

  aabb_state[ index ] =
    aabb_state_type()
      .set_valid( true )
      .set_staging_index( staging_index )
      .set_write_request_index( write_request_index );
  aabb_descriptor desc(
    new aabb_index_t( index ),
    [self=shared_from_this()]( const aabb_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  aabb_state[ index ].set_self( desc.get_weak() );
  return desc;
}

aabb_pool::aabb_descriptor aabb_pool::state_type::allocate( const aabb_descriptor &local, const matrix_pool::matrix_descriptor &matrix ) {
  if( execution_pending ) {
    throw exception::runtime_error( "aabb_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  auto update_requests = update_request_buffer->map< update_request >();

  const aabb_index_t index = allocate_index();

  const request_index_t update_request_index = update_request_index_allocator.allocate();
  
  aabb_state[ index ] =
    aabb_state_type()
      .set_valid( true )
      .set_matrix( matrix )
      .set_local( local )
      .set_update_request_index( update_request_index );
  
  update_requests[ update_request_index ] =
    update_request()
      .set_matrix( *matrix )
      .set_local( *local )
      .set_world( index );
  aabb_descriptor desc(
    new aabb_index_t( index ),
    [self=shared_from_this()]( const aabb_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  aabb_state[ index ].set_self( desc.get_weak() );
  return desc;
}

void aabb_pool::state_type::release( aabb_index_t index ) {
  aabb_descriptor local;
  matrix_pool::matrix_descriptor matrix;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( aabb_state.size() <= index || !aabb_state[ index ].valid ) {
      return;
    }
    release_index( index );
    matrix = aabb_state[ index ].matrix;
    local = aabb_state[ index ].local;
    aabb_state[ index ] = aabb_state_type();
  }
}

void aabb_pool::state_type::touch( const aabb_descriptor &desc ) {
  if( execution_pending ) {
    throw exception::runtime_error( "aabb_pool::state_type::touch : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( aabb_state.size() <= *desc || !aabb_state[ *desc ].valid ) {
    return;
  }
  if( aabb_state[ *desc ].update_request_index ) {
    return;
  }
  const auto &s = aabb_state[ *desc ];
  if( s.local ) {
    const request_index_t update_request_index = update_request_index_allocator.allocate();
    update_request_buffer->map< update_request >()[ update_request_index ] =
      update_request()
        .set_matrix( *s.matrix )
        .set_local( *s.local )
        .set_world( *desc );
    used_on_gpu.push_back( desc );
  }
}

void aabb_pool::state_type::touch( aabb_index_t index ) {
  if( aabb_state.size() <= index || !aabb_state[ index ].valid ) {
    return;
  }
  if( aabb_state[ index ].update_request_index ) {
    return;
  }
  const auto &s = aabb_state[ index ];
  auto desc = aabb_descriptor( s.self );
  touch( desc );
}

void aabb_pool::state_type::set( const aabb_descriptor &desc, const aabb_type &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "aabb_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( aabb_state.size() <= *desc || !aabb_state[ *desc ].valid ) {
    return;
  }
  auto staging = staging_aabb->map< aabb_type >();
  auto write_requests = write_request_buffer->map< write_request >();
  auto &s = aabb_state[ *desc ];
  if( s.staging_index && s.write_request_index ) {
    staging[ *s.staging_index ] = value;
  }
  else if( s.staging_index ) {
    staging[ *s.staging_index ] = value;
    const request_index_t write_request_index = write_request_index_allocator.allocate();
    write_requests[ write_request_index ] =
      write_request()
        .set_staging( *s.staging_index )
        .set_destination( *desc );
    s.set_write_request_index( write_request_index );
  }
  else {
    const aabb_index_t staging_index = staging_index_allocator.allocate();
    staging[ staging_index ] = value;
    const request_index_t write_request_index = write_request_index_allocator.allocate();
    write_requests[ write_request_index ] =
      write_request()
        .set_staging( staging_index )
        .set_destination( *desc );
    s.set_write_request_index( write_request_index );
    s.set_staging_index( staging_index );
    used_on_gpu.push_back( desc );
  }
}

void aabb_pool::state_type::get( const aabb_descriptor &desc, const std::function< void( vk::Result, const aabb_type& ) > &cb ) {
  if( execution_pending ) {
    throw exception::runtime_error( "aabb_pool::state_type::get : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( aabb_state.size() <= *desc || !aabb_state[ *desc ].valid ) {
    return;
  }
  auto read_requests = read_request_buffer->map< read_request >();
  auto &s = aabb_state[ *desc ];
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
    const aabb_index_t staging_index = staging_index_allocator.allocate();
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

aabb_pool::state_type::state_type( const aabb_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_aabb_count ) ),
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_aabb_count ) ),
  write_request_index_allocator( linear_allocator_create_info().set_max( ci.max_aabb_count ) ),
  read_request_index_allocator( linear_allocator_create_info().set_max( ci.max_aabb_count ) ),
  update_request_index_allocator( linear_allocator_create_info().set_max( ci.max_aabb_count ) )
{
  aabb = props.allocator->create_buffer( sizeof( aabb_type ) * props.max_aabb_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY );
  staging_aabb = props.allocator->create_buffer( sizeof( aabb_type ) * props.max_aabb_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  {
    auto mapped = staging_aabb->map< float >();
    std::fill( mapped.begin(), mapped.end(), 1.8f );
  }
  write_request_buffer = props.allocator->create_buffer( sizeof( write_request ) * props.max_aabb_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  read_request_buffer = props.allocator->create_buffer( sizeof( read_request ) * props.max_aabb_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  update_request_buffer = props.allocator->create_buffer( sizeof( update_request ) * props.max_aabb_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  {
    auto mapped = update_request_buffer->map< std::uint32_t >();
    std::fill( mapped.begin(), mapped.end(), 2u );
  }
  write.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.write_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.aabb_buffer_name, aabb } )
      .add_resource( { props.staging_aabb_buffer_name, staging_aabb } )
      .add_resource( { props.write_request_buffer_name, write_request_buffer } )
  ) );
  read.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.read_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.aabb_buffer_name, aabb } )
      .add_resource( { props.staging_aabb_buffer_name, staging_aabb } )
      .add_resource( { props.read_request_buffer_name, read_request_buffer } )
  ) );
  update.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.update_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.aabb_buffer_name, aabb } )
      .add_resource( { props.update_request_buffer_name, update_request_buffer } )
      .add_resource( { props.matrix_buffer_name, props.matrix_pool } )
  ) );
}

aabb_pool::aabb_descriptor aabb_pool::state_type::get_local( const aabb_descriptor &desc ) {
  if( aabb_state.size() <= *desc || !aabb_state[ *desc ].valid ) {
    return desc;
  }
  const auto &s = aabb_state[ *desc ];
  if( s.local ) {
    return s.local;
  }
  return desc;
}

void aabb_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
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
  rec.barrier( { aabb }, {} );
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
  rec.barrier( { aabb }, {} );
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
  rec.compute_barrier( { aabb }, {} );
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< std::function< void() > > cbs;
      std::vector< aabb_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        auto staging = self->staging_aabb->map< aabb_type >();
        for( const auto &desc: self->used_on_gpu ) {
          if( self->aabb_state.size() > *desc && self->aabb_state[ *desc ].valid ) {
            auto &s = self->aabb_state[ *desc ];
            if( s.read_request_index && s.staging_index ) {
              const auto corresponding = self->cbs.equal_range( *desc );
              if( corresponding.first != corresponding.second ) {
                if( result == vk::Result::eSuccess ) {
                  auto aabb = staging[ *s.staging_index ];
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result,aabb]() { cb( result, aabb ); } );
                  }
                }
                else {
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result]() { cb( result, aabb_type() ); } );
                  }
                }
              }
            }
            s.write_request_index = std::nullopt;
            s.update_request_index = std::nullopt;
            s.read_request_index = std::nullopt;
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

aabb_pool::aabb_pool( const aabb_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

aabb_pool::aabb_descriptor aabb_pool::allocate( const aabb_type &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( value );
}
aabb_pool::aabb_descriptor aabb_pool::allocate( const aabb_descriptor &local, const matrix_pool::matrix_descriptor &matrix ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( local, matrix );
}
void aabb_pool::touch( const aabb_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->touch( desc );
}
void aabb_pool::set( const aabb_descriptor &desc, const aabb_type &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->set( desc, value );
}
void aabb_pool::get( const aabb_descriptor &desc, const std::function< void( vk::Result, const aabb_type& ) > &cb ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->get( desc, cb );
}
aabb_pool::aabb_descriptor aabb_pool::get_local( const aabb_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get_local( desc );
}
void aabb_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}
void aabb_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "aabb_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->aabb_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->aabb_state[ i ].staging_index ) {
        temp[ "staging_index" ] = *state->aabb_state[ i ].staging_index;
      }
      if( state->aabb_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->aabb_state[ i ].write_request_index;
      }
      if( state->aabb_state[ i ].read_request_index ) {
        temp[ "read_request_index" ] = *state->aabb_state[ i ].read_request_index;
      }
      if( state->aabb_state[ i ].update_request_index ) {
        temp[ "update_request_index" ] = *state->aabb_state[ i ].update_request_index;
      }
      if( state->aabb_state[ i ].matrix ) {
        temp[ "matrix" ] = *state->aabb_state[ i ].matrix;
      }
      if( state->aabb_state[ i ].local ) {
        temp[ "local" ] = *state->aabb_state[ i ].local;
      }
      {
        auto d = state->aabb_state[ i ].self.lock();
        if( d ) {
          temp[ "self" ] = *d;
        }
      }
      dest[ "aabb_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "index_allocator" ] = state->index_allocator;
  dest[ "staging_aabb" ] = nlohmann::json::array();
  {
    auto m = state->staging_aabb->map< aabb_type >();
    for( std::uint32_t i = 0u; i != state->staging_index_allocator.get_tail(); ++i ) {
      auto e = m[ i ];
      auto temp = nlohmann::json::object();
      temp[ "min" ] = nlohmann::json::array();
      temp[ "min" ].push_back( e.min[ 0 ] );
      temp[ "min" ].push_back( e.min[ 1 ] );
      temp[ "min" ].push_back( e.min[ 2 ] );
      temp[ "min" ].push_back( e.min[ 3 ] );
      temp[ "max" ] = nlohmann::json::array();
      temp[ "max" ].push_back( e.min[ 0 ] );
      temp[ "max" ].push_back( e.min[ 1 ] );
      temp[ "max" ].push_back( e.min[ 2 ] );
      temp[ "max" ].push_back( e.min[ 3 ] );
      dest[ "staging_aabb" ].push_back( temp );
    }
  }
  dest[ "aabb" ] = *state->aabb;
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
      temp[ "matrix" ] = e.matrix;
      temp[ "local" ] = e.local;
      temp[ "world" ] = e.world;
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
  dest[ "write" ] = *state->write;
  dest[ "read" ] = *state->read;
  dest[ "update" ] = *state->update;
  dest[ "execution_pending" ] = state->execution_pending;
}
void to_json( nlohmann::json &dest, const aabb_pool &src ) {
  src.to_json( dest );
}

void test_aabb_pool(
  matrix_pool &matrix,
  aabb_pool &aabb,
  queue_t &queue
) {
  std::uniform_real_distribution<float> dist( -5.0f, 5.0f );
  std::mt19937 engine( std::random_device{}() );
  std::vector< glm::mat4 > l0m;
  std::vector< matrix_pool::matrix_descriptor > l0mdesc;
  for( unsigned int i = 0u; i != 5u; ++i ) {
    const auto m = generate_random_matrix( dist, engine );
    l0m.push_back( m );
    l0mdesc.push_back( matrix.allocate( m ) );
  }
  std::vector< aabb4 > l0a;
  std::vector< matrix_pool::matrix_descriptor > l0adesc;
  for( unsigned int i = 0u; i != 5u; ++i ) {
    const auto a = generate_random_aabb( dist, engine );
    l0a.push_back( a );
    l0adesc.push_back( aabb.allocate( a ) );
    aabb.get(
      l0adesc.back(),
      [expected=l0a.back(),s=i]( vk::Result result, const aabb4 &v ) {
        if( result != vk::Result::eSuccess ) {
          std::cout << "expected : " << nlohmann::json( expected ).dump() << std::endl;
          std::cout << "result[" << s << "] : failed" << std::endl;
        }
        else if( !similar_aabb( expected, v ) ) {
          std::cout << "expected : " << nlohmann::json( expected ).dump() << std::endl;
          std::cout << "result[" << s << "] : " << nlohmann::json( v ).dump() << std::endl;
        }
      }
    );
  }
  std::vector< aabb4 > l1a;
  std::vector< matrix_pool::matrix_descriptor > l1adesc;
  for( unsigned int i = 0u; i != 5u; ++i ) {
    l1adesc.push_back( aabb.allocate( l0adesc[ i ], l0mdesc[ 2 ] ) );
    aabb.get(
      l1adesc.back(),
      [expected= l0m[ 2 ] * l0a[ i ],s=i]( vk::Result result, const aabb4 &v ) {
        if( result != vk::Result::eSuccess ) {
          std::cout << "expected : " << nlohmann::json( expected ).dump() << std::endl;
          std::cout << "result[" << s << "] : failed" << std::endl;
        }
        else if( !similar_aabb( expected, v ) ) {
          std::cout << "expected : " << nlohmann::json( expected ).dump() << std::endl;
          std::cout << "result[" << s << "] : " << nlohmann::json( v ).dump() << std::endl;
        }
      }
    );
  }
  {
    auto command_buffer = queue.get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      matrix( rec );
      aabb( rec );
    }
    command_buffer->execute_and_wait();
  }
}

}

