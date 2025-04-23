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
#include <gct/shader_module_reflection.hpp>
#include <gct/shader_module.hpp>
#include <gct/buffer_pool.hpp>
namespace gct {

buffer_pool::buffer_index_t buffer_pool::state_type::allocate_index( std::uint32_t count ) {
  const auto index = index_allocator.allocate( count );
  if( index >= buffer_state.size() ) {
    buffer_state.resize( index + count );
  }
  return index; 
}

void buffer_pool::state_type::release_index( buffer_pool::buffer_index_t index ) {
  if( index >= buffer_state.size() || !buffer_state[ index ].valid ) {
    return;
  }
  buffer_state[ index ].valid = false;
  index_allocator.release( index );
}

buffer_pool::buffer_descriptor buffer_pool::state_type::allocate( std::uint32_t count, const std::uint8_t *begin, const std::uint8_t *end ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( count == 0u ) {
    throw exception::invalid_argument( "buffer_pool::state_type::allocate : count == 0 is not allowed", __FILE__, __LINE__ );
  }

  auto write_requests = write_request_buffer->map< write_request >();
  auto staging = staging_buffer->map< std::uint8_t >();

  const buffer_index_t index = allocate_index( count );

  const buffer_index_t staging_index = staging_index_allocator.allocate( count );

  const request_index_t write_request_index = write_request_index_allocator.allocate( count );

  for( std::uint32_t i = 0u; i != count; ++i ) {
    write_requests[ write_request_index + i ] =
      write_request()
        .set_staging( staging_index + i )
        .set_destination( index + i );
    std::copy( begin, end, std::next( staging.begin(), index * aligned_size ) );
    buffer_state[ index + i ] =
      buffer_state_type()
        .set_valid( true )
        .set_staging_index( staging_index + i )
        .set_write_request_index( write_request_index + i );
  }

  buffer_descriptor desc(
    new buffer_index_t( index ),
    [self=shared_from_this()]( const buffer_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  buffer_state[ index ].set_self( desc.get_weak() );
  used_on_gpu.push_back( desc );
  return desc;
}

buffer_pool::buffer_descriptor buffer_pool::state_type::allocate( std::uint32_t count ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( count == 0u ) {
    throw exception::invalid_argument( "buffer_pool::state_type::allocate : count == 0 is not allowed", __FILE__, __LINE__ );
  }

  auto write_requests = write_request_buffer->map< write_request >();
  auto staging = staging_buffer->map< std::uint8_t >();

  const buffer_index_t index = allocate_index( count );

  const buffer_index_t staging_index = staging_index_allocator.allocate( count );

  const request_index_t write_request_index = write_request_index_allocator.allocate( count );
  for( std::uint32_t i = 0u; i != count; ++i ) {
    write_requests[ write_request_index + i ] =
      write_request()
        .set_staging( staging_index + i )
        .set_destination( index + i );
    std::fill( std::next( staging.begin(), index * aligned_size ), std::next( staging.begin(), ( index + i + 1u ) * aligned_size ), 0u );
    buffer_state[ index + i ] =
      buffer_state_type()
        .set_valid( true )
        .set_staging_index( staging_index + i )
        .set_write_request_index( write_request_index + i );
  }

  buffer_descriptor desc(
    new buffer_index_t( index ),
    [self=shared_from_this()]( const buffer_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  buffer_state[ index ].set_self( desc.get_weak() );
  used_on_gpu.push_back( desc );
  return desc;
}


void buffer_pool::state_type::release( buffer_index_t index ) {
  std::lock_guard< std::mutex > lock( guard );
  if( buffer_state.size() <= index || !buffer_state[ index ].valid ) {
    return;
  }
  release_index( index );
  buffer_state[ index ] = buffer_state_type();
}

void buffer_pool::state_type::set( const buffer_descriptor &desc, std::uint32_t index, const std::uint8_t *begin, const std::uint8_t *end ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( buffer_state.size() <= ( *desc + index ) || !buffer_state[ *desc + index ].valid ) {
    return;
  }
  {
    auto write_requests = write_request_buffer->map< write_request >();
    auto staging = staging_buffer->map< std::uint8_t >();
    auto &s = buffer_state[ *desc + index ];
    if( s.staging_index && s.write_request_index ) {
      std::copy( begin, end, std::next( staging.begin(), *s.staging_index * aligned_size ) );
    }
    else if( s.staging_index ) {
      std::copy( begin, end, std::next( staging.begin(), *s.staging_index * aligned_size ) );
      const request_index_t write_request_index = write_request_index_allocator.allocate();
      write_requests[ write_request_index ] =
        write_request()
          .set_staging( *s.staging_index )
          .set_destination( *desc + index );
      s.set_write_request_index( write_request_index );
    }
    else {
      const buffer_index_t staging_index = staging_index_allocator.allocate();
      std::copy( begin, end, std::next( staging.begin(), staging_index * aligned_size ) );
      const request_index_t write_request_index = write_request_index_allocator.allocate();
      write_requests[ write_request_index ] =
        write_request()
          .set_staging( staging_index )
          .set_destination( *desc + index );
      s.set_write_request_index( write_request_index );
      s.set_staging_index( staging_index );
      used_on_gpu.push_back( desc );
    }
  }
}

void buffer_pool::state_type::clear( const buffer_descriptor &desc, std::uint32_t index ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( buffer_state.size() <= ( *desc + index ) || !buffer_state[ *desc + index ].valid ) {
    return;
  }
  {
    auto write_requests = write_request_buffer->map< write_request >();
    auto staging = staging_buffer->map< std::uint8_t >();
    auto &s = buffer_state[ *desc + index ];
    if( s.staging_index && s.write_request_index ) {
      std::fill( std::next( staging.begin(), *s.staging_index * aligned_size ), std::next( staging.begin(), ( *s.staging_index + 1u ) * aligned_size ), 0u );
    }
    else if( s.staging_index ) {
      std::fill( std::next( staging.begin(), *s.staging_index * aligned_size ), std::next( staging.begin(), ( *s.staging_index + 1u ) * aligned_size ), 0u );
      const request_index_t write_request_index = write_request_index_allocator.allocate();
      write_requests[ write_request_index ] =
        write_request()
          .set_staging( *s.staging_index )
          .set_destination( *desc + index );
      s.set_write_request_index( write_request_index );
    }
    else {
      const buffer_index_t staging_index = staging_index_allocator.allocate();
      std::fill( std::next( staging.begin(), staging_index * aligned_size ), std::next( staging.begin(), ( staging_index + 1u ) * aligned_size ), 0u );
      const request_index_t write_request_index = write_request_index_allocator.allocate();
      write_requests[ write_request_index ] =
        write_request()
          .set_staging( staging_index )
          .set_destination( *desc + index );
      s.set_write_request_index( write_request_index );
      s.set_staging_index( staging_index );
      used_on_gpu.push_back( desc );
    }
  }
}


void buffer_pool::state_type::clear() {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  for( auto &v: buffer_state ) {
    if( v.valid ) {
      clear( buffer_descriptor( v.self.lock() ), 0u );
    }
  }
}

std::uint32_t buffer_pool::state_type::size() const {
  return index_allocator.get_tail();
}


void buffer_pool::state_type::get( const buffer_descriptor &desc, std::uint32_t index, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) > &cb ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::get : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( buffer_state.size() <= ( *desc + index ) || !buffer_state[ *desc + index ].valid ) {
    return;
  }
  {
    auto read_requests = read_request_buffer->map< read_request >();
    auto &s = buffer_state[ *desc + index ];
    if( s.staging_index && s.read_request_index ) {
      cbs.insert( std::make_pair( *desc + index, cb ) );
    }
    else if( s.staging_index ) {
      cbs.insert( std::make_pair( *desc + index, cb ) );
      const request_index_t read_request_index = read_request_index_allocator.allocate();
      read_requests[ read_request_index ] =
        read_request()
          .set_source( *desc + index )
          .set_staging( *s.staging_index );
      s.set_read_request_index( read_request_index );
    }
    else {
      cbs.insert( std::make_pair( *desc + index, cb ) );
      const buffer_index_t staging_index = staging_index_allocator.allocate();
      const request_index_t read_request_index = read_request_index_allocator.allocate();
      read_requests[ read_request_index ] =
        read_request()
          .set_source( *desc + index )
          .set_staging( staging_index );
      s.set_read_request_index( read_request_index );
      s.set_staging_index( staging_index );
      used_on_gpu.push_back( desc );
    }
  }
}
bool buffer_pool::state_type::is_valid( const buffer_descriptor &desc ) const {
  if( buffer_state.size() <= *desc || !buffer_state[ *desc ].valid ) {
    return false;
  }
  return true;
}

bool buffer_pool::is_valid( const buffer_descriptor &desc ) const {
  return state->is_valid( desc );
}

buffer_pool::state_type::state_type( const buffer_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_buffer_count ) ),
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_buffer_count ) ),
  write_request_index_allocator( linear_allocator_create_info().set_max( ci.max_buffer_count ) ),
  read_request_index_allocator( linear_allocator_create_info().set_max( ci.max_buffer_count ) )
{
  const auto reflection = shader_module_reflection_t( std::filesystem::path( props.write_shader ) );
  aligned_size = reflection.get_member_pointer( props.buffer_name, props.layout ).get_stride();
  buffer = props.allocator_set.allocator->create_buffer(
    aligned_size * props.max_buffer_count,
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferSrc|
    vk::BufferUsageFlagBits::eTransferDst,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  staging_buffer = props.allocator_set.allocator->create_buffer( aligned_size * props.max_buffer_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  write_request_buffer = props.allocator_set.allocator->create_buffer( sizeof( write_request ) * props.max_buffer_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  read_request_buffer = props.allocator_set.allocator->create_buffer( sizeof( read_request ) * props.max_buffer_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  write.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.write_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.buffer_name, buffer } )
      .add_resource( { props.staging_buffer_name, staging_buffer } )
      .add_resource( { props.write_request_buffer_name, write_request_buffer } )
  ) );
  read.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.read_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.buffer_name, buffer } )
      .add_resource( { props.staging_buffer_name, staging_buffer } )
      .add_resource( { props.read_request_buffer_name, read_request_buffer } )
  ) );
}

void buffer_pool::state_type::flush( command_buffer_recorder_t &rec ) {
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
  rec.barrier( { buffer }, {} );
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
  rec.barrier( { buffer }, {} );
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< std::function< void() > > cbs;
      std::vector< buffer_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        auto staging = self->staging_buffer->map< std::uint8_t >();
        for( const auto &desc: self->used_on_gpu ) {
          if( self->buffer_state.size() > *desc && self->buffer_state[ *desc ].valid ) {
            auto &s = self->buffer_state[ *desc ];
            if( s.read_request_index && s.staging_index ) {
              const auto corresponding = self->cbs.equal_range( *desc );
              if( corresponding.first != corresponding.second ) {
                if( result == vk::Result::eSuccess ) {
                  std::vector< std::uint8_t > temp( self->aligned_size );
                  const auto begin = std::next( staging.begin(), self->aligned_size * *s.staging_index );
                  const auto end = std::next( begin, self->aligned_size );
                  std::copy( begin, end, temp.begin() );
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result,temp=temp]() mutable { cb( result, std::move( temp ) ); } );
                  }
                }
                else {
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result]() { cb( result, std::vector< std::uint8_t >{} ); } );
                  }
                }
              }
            }
            s.write_request_index = std::nullopt;
            s.read_request_index = std::nullopt;
            s.staging_index = std::nullopt;
          }
        }
        self->write_request_index_allocator.reset();
        self->read_request_index_allocator.reset();
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

buffer_pool::buffer_pool( const buffer_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

buffer_pool::buffer_descriptor buffer_pool::allocate( const std::uint8_t *begin, const std::uint8_t *end ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( 1u, begin, end );
}

buffer_pool::buffer_descriptor buffer_pool::allocate( std::uint32_t count, const std::uint8_t *begin, const std::uint8_t *end ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( count, begin, end );
}

buffer_pool::buffer_descriptor buffer_pool::allocate( std::uint32_t count ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( count );
}

void buffer_pool::set( const buffer_descriptor &desc, const std::uint8_t *begin, const std::uint8_t *end ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->set( desc, 0u, begin, end );
}

void buffer_pool::set( const buffer_descriptor &desc, std::uint32_t index,  const std::uint8_t *begin, const std::uint8_t *end ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->set( desc, index, begin, end );
}


void buffer_pool::clear( const buffer_descriptor &desc, std::uint32_t index ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->clear( desc, index );
}

void buffer_pool::clear() {
  std::lock_guard< std::mutex > lock( state->guard );
  state->clear();
}

std::uint32_t buffer_pool::size() const {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->size();
}

void buffer_pool::get( const buffer_descriptor &desc, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) > &cb ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->get( desc, 0u, cb );
}
void buffer_pool::get( const buffer_descriptor &desc, std::uint32_t index, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) > &cb ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->get( desc, index, cb );
}
void buffer_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}
void buffer_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "buffer_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->buffer_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->buffer_state[ i ].staging_index ) {
        temp[ "staging_index" ] = *state->buffer_state[ i ].staging_index;
      }
      if( state->buffer_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->buffer_state[ i ].write_request_index;
      }
      if( state->buffer_state[ i ].read_request_index ) {
        temp[ "read_request_index" ] = *state->buffer_state[ i ].read_request_index;
      }
      dest[ "buffer_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "index_allocator" ] = state->index_allocator;
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
  dest[ "staging_index_allocator" ] = state->staging_index_allocator;
  dest[ "write_request_index_allocator" ] = state->write_request_index_allocator;
  dest[ "read_request_index_allocator" ] = state->read_request_index_allocator;
  dest[ "used_on_gpu" ] = nlohmann::json::array();
  for( const auto &e: state->used_on_gpu ) {
    dest[ "used_on_gpu" ].push_back( *e );
  }
  dest[ "write" ] = *state->write;
  dest[ "read" ] = *state->read;
  dest[ "aligned_size" ] = state->aligned_size;
  dest[ "member_pointer" ] = get_member_pointer();
  dest[ "execution_pending" ] = state->execution_pending;
}
spv_member_pointer buffer_pool::get_member_pointer() const {
  return state->write->get_pipeline()->get_props().get_stage().get_shader_module()->get_props().get_reflection().get_member_pointer( state->props.buffer_name, state->props.layout )[ 0 ];
}
void to_json( nlohmann::json &dest, const buffer_pool &src ) {
  src.to_json( dest );
}

}

