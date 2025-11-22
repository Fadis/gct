#include <boost/range/iterator_range.hpp>
#include <nlohmann/json.hpp>
#include <vulkan2json/BufferCopy.hpp>
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
#include <gct/simplify_buffer_copy.hpp>
#include <gct/shader_module_reflection.hpp>
namespace gct {

buffer_pool::buffer_index_t buffer_pool::state_type::allocate_index( std::uint32_t count ) {
  const auto index = index_allocator.allocate( count );
  return index; 
}

void buffer_pool::state_type::release_index( buffer_pool::buffer_index_t index ) {
  if( ( buffer_state.find( index ) == buffer_state.end() ) ) {
    return;
  }
  index_allocator.release( index );
}

buffer_pool::buffer_descriptor buffer_pool::state_type::allocate( std::uint32_t count, const std::uint8_t *begin, const std::uint8_t *end ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( count == 0u ) {
    throw exception::invalid_argument( "buffer_pool::state_type::allocate : count == 0 is not allowed", __FILE__, __LINE__ );
  }

  auto staging = staging_buffer->map< std::uint8_t >();

  const buffer_index_t index = allocate_index( count );

  const buffer_index_t staging_index_ = staging_index_allocator.allocate( count );

  const request_index_t request_index = write_request_index_allocator.allocate( count );

  for( std::uint32_t i = 0u; i != count; ++i ) {
    std::copy( begin, end, std::next( staging.begin(), index * aligned_size ) );
  }
  write_region.push_back(
    vk::BufferCopy()
      .setSrcOffset( index * aligned_size )
      .setDstOffset( staging_index_ * aligned_size )
      .setSize( count * aligned_size )
  );
  buffer_state[ index ] =
    buffer_state_type();
  
  for( std::uint32_t i = 0u; i != count; ++i ) {
    staging_index.insert( std::make_pair( index + i, staging_index_ + i ) );
    write_request_index.insert( std::make_pair( index + i, request_index + i ) );
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

  auto staging = staging_buffer->map< std::uint8_t >();

  const buffer_index_t index = allocate_index( count );

  buffer_state[ index ] =
    buffer_state_type();

  fill_requests.add( index * aligned_size, count * aligned_size );

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
  if( ( buffer_state.find( index ) == buffer_state.end() ) ) {
    return;
  }
  release_index( index );
  buffer_state.erase( index );
}

void buffer_pool::state_type::set( const buffer_descriptor &desc, std::uint32_t index, const std::uint8_t *begin, const std::uint8_t *end ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( ( buffer_state.find( *desc ) == buffer_state.end() ) ) {
    return;
  }
  {
    auto staging = staging_buffer->map< std::uint8_t >();
    auto &s = buffer_state[ *desc ];
    const auto si = staging_index.find( *desc + index );
    const auto wri = write_request_index.find( *desc + index );

    if( si != staging_index.end() && wri != write_request_index.end() ) {
      std::copy( begin, end, std::next( staging.begin(), si->second * aligned_size ) );
    }
    else if( si != staging_index.end() ) {
      std::copy( begin, end, std::next( staging.begin(), si->second * aligned_size ) );
      const request_index_t request_index = write_request_index_allocator.allocate();
      write_request_index.insert( std::make_pair( *desc + index, request_index ) );
      write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( si->second * aligned_size )
          .setDstOffset( ( *desc + index ) * aligned_size )
          .setSize( aligned_size )
      );
    }
    else {
      const buffer_index_t staging_index_ = staging_index_allocator.allocate();
      std::copy( begin, end, std::next( staging.begin(), staging_index_ * aligned_size ) );
      const request_index_t request_index = write_request_index_allocator.allocate();
      write_request_index.insert( std::make_pair( *desc + index, request_index ) );
      write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( staging_index_ * aligned_size )
          .setDstOffset( ( *desc + index ) * aligned_size )
          .setSize( aligned_size )
      );
      staging_index.insert( std::make_pair( *desc + index, staging_index_ ) );
      used_on_gpu.push_back( desc );
    }
  }
}

void buffer_pool::state_type::clear( const buffer_descriptor &desc, std::uint32_t index, std::uint32_t count ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( ( buffer_state.find( *desc ) == buffer_state.end() ) ) {
    return;
  }
  if( count == 0u && index == 0u ) {
    count = index_allocator.get_size( *desc );
  }
  {
    auto &s = buffer_state[ *desc ];
    const auto si = staging_index.find( *desc + index );
    const auto wri = write_request_index.find( *desc + index );
    if( si != staging_index.end() && wri != write_request_index.end() ) {
      auto staging = staging_buffer->map< std::uint8_t >();
      std::fill( std::next( staging.begin(), si->second * aligned_size ), std::next( staging.begin(), ( si->second + 1u ) * aligned_size ), 0u );
    }
    else if( si != staging_index.end() ) {
      auto staging = staging_buffer->map< std::uint8_t >();
      std::fill( std::next( staging.begin(), si->second * aligned_size ), std::next( staging.begin(), ( si->second + 1u ) * aligned_size ), 0u );
      const request_index_t request_index = write_request_index_allocator.allocate();
      write_request_index.insert( std::make_pair( *desc + index, request_index ) );
      write_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( si->second * aligned_size )
          .setDstOffset( ( *desc + index ) * aligned_size )
          .setSize( aligned_size )
      );
    }
    else {
      fill_requests.add( ( *desc + index ) * aligned_size, count * aligned_size );
      used_on_gpu.push_back( desc );
    }
  }
}

void buffer_pool::state_type::clear() {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  for( auto &v: buffer_state ) {
    auto d = v.second.self.lock();
    clear( buffer_descriptor( d ), 0u, index_allocator.get_size( *d ) );
  }
}

std::uint32_t buffer_pool::state_type::size() const {
  return index_allocator.get_tail();
}


void buffer_pool::state_type::get( const buffer_descriptor &desc, std::uint32_t index, const std::function< void( vk::Result, std::vector< std::uint8_t >&& ) > &cb ) {
  if( execution_pending ) {
    throw exception::runtime_error( "buffer_pool::state_type::get : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( ( buffer_state.find( *desc ) == buffer_state.end() ) ) {
    return;
  }
  {
    auto &s = buffer_state[ *desc ];
    const auto si = staging_index.find( *desc + index );
    const auto rri = read_request_index.find( *desc + index );
    if( si != staging_index.end() && rri != read_request_index.end() ) {
      cbs.insert( std::make_pair( *desc + index, cb ) );
    }
    else if( si != staging_index.end() ) {
      cbs.insert( std::make_pair( *desc + index, cb ) );
      const request_index_t request_index = read_request_index_allocator.allocate();
      read_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( ( *desc + index ) * aligned_size )
          .setDstOffset( si->second * aligned_size )
          .setSize( aligned_size )
      );
      read_request_index.insert( std::make_pair( *desc + index, request_index ) );
    }
    else {
      cbs.insert( std::make_pair( *desc + index, cb ) );
      const buffer_index_t staging_index_ = staging_index_allocator.allocate();
      const request_index_t request_index = read_request_index_allocator.allocate();
      read_region.push_back(
        vk::BufferCopy()
          .setSrcOffset( ( *desc + index ) * aligned_size )
          .setDstOffset( staging_index_ * aligned_size )
          .setSize( aligned_size )
      );
      read_request_index.insert( std::make_pair( *desc + index, request_index ) );
      staging_index.insert( std::make_pair( *desc + index, staging_index_ ) );
      used_on_gpu.push_back( desc );
    }
  }
}
bool buffer_pool::state_type::is_valid( const buffer_descriptor &desc ) const {
  if( ( buffer_state.find( *desc ) == buffer_state.end() ) ) {
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
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_request_count ) ),
  write_request_index_allocator( linear_allocator_create_info().set_max( ci.max_request_count ) ),
  read_request_index_allocator( linear_allocator_create_info().set_max( ci.max_request_count ) )
{
  if( std::filesystem::exists( props.dummy_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.dummy_shader )
    );
  }
  else if( std::filesystem::exists( props.write_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.write_shader )
    );
  }
  else if( std::filesystem::exists( props.read_shader ) ) {
    reflection.reset(
      new shader_module_reflection_t( props.read_shader )
    );
  }
  else {
    throw exception::runtime_error( "buffer_pool::state_type::state_type : at least one shader is required", __FILE__, __LINE__ );
  }
  aligned_size = reflection->get_member_pointer( props.buffer_name, props.layout ).get_stride();
  buffer = props.allocator_set.allocator->create_buffer(
    aligned_size * props.max_buffer_count,
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferSrc|
    vk::BufferUsageFlagBits::eTransferDst,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  staging_buffer = props.allocator_set.allocator->create_buffer(
    aligned_size * props.max_request_count,
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferSrc|
    vk::BufferUsageFlagBits::eTransferDst,
    VMA_MEMORY_USAGE_CPU_TO_GPU
  );

  //const buffer_index_t dummy = allocate_index( 1 );
}

void buffer_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  simplify_buffer_copy( write_region );
  simplify_buffer_copy( read_region );
  rec.barrier( { buffer }, {} );
  if( !read_region.empty() ) {
    rec->copyBuffer(
      **buffer,
      **staging_buffer,
      read_region
    );
  }
  rec.barrier( { buffer }, {} );
  if( !fill_requests.get().empty() ) {
    for( const auto &i: fill_requests.get() ) {
      rec->fillBuffer( **buffer, i.first, i.second, 0u );
    }
    rec.transfer_barrier( { buffer }, {} );
  }
  if( !write_region.empty() ) {
    rec->copyBuffer(
      **staging_buffer,
      **buffer,
      write_region
    );
    rec.transfer_barrier( { buffer }, {} );
  }
  rec.barrier( { buffer }, {} );
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< std::function< void() > > cbs;
      std::vector< buffer_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        auto staging = self->staging_buffer->map< std::uint8_t >();
        std::sort( self->used_on_gpu.begin(), self->used_on_gpu.end() );
        self->used_on_gpu.erase( std::unique( self->used_on_gpu.begin(), self->used_on_gpu.end() ), self->used_on_gpu.end() );
        for( const auto &desc: self->used_on_gpu ) {
          if( ( self->buffer_state.find( *desc ) != self->buffer_state.end() ) ) {
            auto &s = self->buffer_state[ *desc ];
            const auto begin = self->read_request_index.lower_bound( *desc );
            const auto array_size = self->index_allocator.get_size( *desc );
            const auto end = self->read_request_index.lower_bound( *desc + array_size );
            for( const auto &index_rrr: boost::make_iterator_range( begin, end ) ) {
              const auto &[index,rrr] = index_rrr;
              const auto si = self->staging_index.find( index );
              if( si != self->staging_index.end() ) {
                const auto corresponding = self->cbs.equal_range( index );
                if( corresponding.first != corresponding.second ) {
                  if( result == vk::Result::eSuccess ) {
                    std::vector< std::uint8_t > temp( self->aligned_size );
                    const auto begin = std::next( staging.begin(), self->aligned_size * si->second );
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
            }
          }
        }
        self->staging_index.clear();
        self->read_request_index.clear();
        self->write_request_index.clear();
        self->fill_requests.clear();
        self->write_request_index_allocator.reset();
        self->read_request_index_allocator.reset();
        self->staging_index_allocator.reset();
        self->write_region.clear();
        self->read_region.clear();
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


void buffer_pool::clear( const buffer_descriptor &desc, std::uint32_t index, std::uint32_t count ) {
  if( count == 0u ) return;
  std::lock_guard< std::mutex > lock( state->guard );
  state->clear( desc, index, count );
}

void buffer_pool::clear( const buffer_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->clear( desc, 0u, 0u );
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
  for( const auto &bi_br: state->buffer_state ) {
    const auto &[bi,br] = bi_br;
    auto temp = nlohmann::json::object();
  }
  dest[ "staging_index" ] = nlohmann::json::object();
  for( const auto &index_sr: state->staging_index ) {
    const auto &[index,sr] = index_sr;
    dest[ "staging_index" ][ std::to_string( index ) ] = sr;
  }
  dest[ "write_request_index" ] = nlohmann::json::object();
  for( const auto &index_wrr: state->write_request_index ) {
    const auto &[index,wrr] = index_wrr;
    dest[ "write_request_index" ][ std::to_string( index ) ] = wrr;
  }
  dest[ "read_request_index" ] = nlohmann::json::object();
  for( const auto &index_rrr: state->read_request_index ) {
    const auto &[index,rrr] = index_rrr;
    dest[ "write_request_index" ][ std::to_string( index ) ] = rrr;
  }
  dest[ "index_allocator" ] = state->index_allocator;
  dest[ "write_region" ] = nlohmann::json::array();
  for( const auto &v: state->write_region ) {
    dest[ "write_region" ].push_back( nlohmann::json( v ) );
  }
  dest[ "read_region" ] = nlohmann::json::array();
  for( const auto &v: state->read_region ) {
    dest[ "read_region" ].push_back( nlohmann::json( v ) );
  }
  dest[ "staging_index_allocator" ] = state->staging_index_allocator;
  dest[ "write_request_index_allocator" ] = state->write_request_index_allocator;
  dest[ "read_request_index_allocator" ] = state->read_request_index_allocator;
  dest[ "used_on_gpu" ] = nlohmann::json::array();
  for( const auto &e: state->used_on_gpu ) {
    dest[ "used_on_gpu" ].push_back( *e );
  }
  dest[ "aligned_size" ] = state->aligned_size;
  dest[ "member_pointer" ] = get_member_pointer();
  dest[ "execution_pending" ] = state->execution_pending;
}
spv_member_pointer buffer_pool::get_member_pointer() const {
  return state->reflection->get_member_pointer( state->props.buffer_name, state->props.layout )[ 0 ];
}
void to_json( nlohmann::json &dest, const buffer_pool &src ) {
  src.to_json( dest );
}

}

