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
#include <gct/matrix_pool.hpp>
namespace gct {

matrix_pool::matrix_index_t matrix_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= matrix_state.size() ) {
    matrix_state.resize( index + 1u );
  }
  return index; 
}

void matrix_pool::state_type::release_index( matrix_pool::matrix_index_t index ) {
  if( index >= matrix_state.size() || !matrix_state[ index ].valid ) {
    return;
  }
  matrix_state[ index ].valid = false;
  index_allocator.release( index );
}

matrix_pool::matrix_descriptor matrix_pool::state_type::allocate( const glm::mat4 &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "matrix_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  auto write_requests = write_request_buffer->map< write_request >();
  auto staging = staging_matrix->map< glm::mat4 >();

  const matrix_index_t index = allocate_index();

  const matrix_index_t staging_index = staging_index_allocator.allocate();

  const request_index_t write_request_index = write_request_index_allocator.allocate();

  write_requests[ write_request_index ] =
    write_request()
      .set_staging( staging_index )
      .set_destination( index );

  staging[ staging_index ] = value;

  matrix_state[ index ] =
    matrix_state_type()
      .set_valid( true )
      .set_staging_index( staging_index )
      .set_write_request_index( write_request_index );

  matrix_descriptor desc(
    new matrix_index_t( index ),
    [self=shared_from_this()]( const matrix_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  matrix_state[ index ].set_self( desc.get_weak() );
  return desc;
}
matrix_pool::matrix_descriptor matrix_pool::state_type::allocate( const matrix_descriptor &parent, const glm::mat4 &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "matrix_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( *parent >= matrix_state.size() || !matrix_state[ *parent ].valid ) {
    throw -1;
  }
  
  auto write_requests = write_request_buffer->map< write_request >();
  auto staging = staging_matrix->map< glm::mat4 >();
  
  const std::uint32_t level = matrix_state[ *parent ].level + 1u; 
  if( update_request_list.size() <= level ) {
    update_request_list.resize( level + 1u );
  }

  const matrix_index_t index = allocate_index();
  const matrix_index_t local_index = allocate_index();
  const matrix_index_t staging_index = staging_index_allocator.allocate();
  const request_index_t write_request_index = write_request_index_allocator.allocate();

  write_requests[ write_request_index ] =
    write_request()
      .set_staging( staging_index )
      .set_destination( local_index );

  staging[ staging_index ] = value;

  matrix_state[ local_index ] =
    matrix_state_type()
      .set_valid( true )
      .set_staging_index( staging_index )
      .set_write_request_index( write_request_index );

  matrix_descriptor local_desc(
    new matrix_index_t( local_index ),
    [self=shared_from_this()]( const matrix_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( local_desc );
  matrix_state[ local_index ].set_self( local_desc.get_weak() );

  matrix_state[ index ] =
    matrix_state_type()
      .set_valid( true )
      .set_level( level )
      .set_local( local_desc )
      .set_parent( parent );

  update_request_list[ level ].push_back(
    update_request()
      .set_parent( *parent )
      .set_local( local_index )
      .set_world( index )
  );

  matrix_descriptor desc(
    new matrix_index_t( index ),
    [self=shared_from_this()]( const matrix_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  matrix_state[ index ].set_self( desc.get_weak() );
  return desc;
}

void matrix_pool::state_type::release( matrix_index_t index ) {
  matrix_descriptor local;
  matrix_descriptor parent;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( matrix_state.size() <= index || !matrix_state[ index ].valid ) {
      return;
    }
    release_index( index );
    parent = matrix_state[ index ].parent;
    local = matrix_state[ index ].local;
    matrix_state[ index ] = matrix_state_type();
  }
}

void matrix_pool::state_type::touch( const matrix_descriptor &desc ) {
  if( execution_pending ) {
    throw exception::runtime_error( "matrix_pool::state_type::touch : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return;
  }
  if( update_requested.find( *desc ) != update_requested.end() ) {
    return;
  }
  const auto &s = matrix_state[ *desc ];
  if( s.local ) {
    update_request_list[ s.level ].push_back(
      update_request()
        .set_parent( *s.parent )
        .set_local( *s.local )
        .set_world( *desc )
    );
    update_requested.insert( *desc );
    used_on_gpu.push_back( desc );
  }
  const auto [begin,end] = edge.equal_range( *desc );
  for( auto iter = begin; iter != end; ++iter ) {
    touch( iter->second );
  }
}

void matrix_pool::state_type::touch( matrix_index_t index ) {
  if( matrix_state.size() <= index || !matrix_state[ index ].valid ) {
    return;
  }
  if( update_requested.find( index ) != update_requested.end() ) {
    return;
  }
  const auto &s = matrix_state[ index ];
  auto desc = matrix_descriptor( s.self );
  touch( desc );
}

void matrix_pool::state_type::set( const matrix_descriptor &desc, const glm::mat4 &value ) {
  if( execution_pending ) {
    throw exception::runtime_error( "matrix_pool::state_type::set : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return;
  }
  {
    auto write_requests = write_request_buffer->map< write_request >();
    auto staging = staging_matrix->map< glm::mat4 >();
    auto &s = matrix_state[ *desc ];
    if( s.local ) {
      auto &l = matrix_state[ *s.local ];
      if( l.staging_index && l.write_request_index ) {
        staging[ *l.staging_index ] = value;
      }
      else if( l.staging_index ) {
        staging[ *l.staging_index ] = value;
        const request_index_t write_request_index = write_request_index_allocator.allocate();
        write_requests[ write_request_index ] =
          write_request()
            .set_staging( *l.staging_index )
            .set_destination( *s.local );
        l.set_write_request_index( write_request_index );
      }
      else {
        const matrix_index_t staging_index = staging_index_allocator.allocate();
        staging[ staging_index ] = value;
        const request_index_t write_request_index = write_request_index_allocator.allocate();
        write_requests[ write_request_index ] =
          write_request()
            .set_staging( staging_index )
            .set_destination( *s.local );
        l.set_write_request_index( write_request_index );
        l.set_staging_index( staging_index );
        used_on_gpu.push_back( s.local );
      }
    }
    else {
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
        const matrix_index_t staging_index = staging_index_allocator.allocate();
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
  }
  touch( desc );
}

void matrix_pool::state_type::get( const matrix_descriptor &desc, const std::function< void( vk::Result, const glm::mat4& ) > &cb ) {
  if( execution_pending ) {
    throw exception::runtime_error( "matrix_pool::state_type::get : last execution is not completed yet", __FILE__, __LINE__ );
  }
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return;
  }
  {
    auto read_requests = read_request_buffer->map< read_request >();
    auto &s = matrix_state[ *desc ];
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
      const matrix_index_t staging_index = staging_index_allocator.allocate();
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
}
bool matrix_pool::state_type::is_valid( const matrix_descriptor &desc ) const {
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return false;
  }
  return true;
}

bool matrix_pool::is_valid( const matrix_descriptor &desc ) const {
  return state->is_valid( desc );
}

matrix_pool::state_type::state_type( const matrix_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_matrix_count ) ),
  staging_index_allocator( linear_allocator_create_info().set_max( ci.max_matrix_count ) ),
  write_request_index_allocator( linear_allocator_create_info().set_max( ci.max_matrix_count ) ),
  read_request_index_allocator( linear_allocator_create_info().set_max( ci.max_matrix_count ) )
{
  matrix = props.allocator->create_buffer( sizeof( glm::mat4 ) * props.max_matrix_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_GPU_ONLY );
  staging_matrix = props.allocator->create_buffer( sizeof( glm::mat4 ) * props.max_matrix_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  write_request_buffer = props.allocator->create_buffer( sizeof( write_request ) * props.max_matrix_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  read_request_buffer = props.allocator->create_buffer( sizeof( read_request ) * props.max_matrix_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  update_request_buffer = props.allocator->create_buffer( sizeof( update_request ) * props.max_matrix_count, vk::BufferUsageFlagBits::eStorageBuffer, VMA_MEMORY_USAGE_CPU_TO_GPU );
  write.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.write_shader )
      .set_swapchain_image_count( 1u )
      .set_external_descriptor_set( props.external_descriptor_set )
      .set_resources( props.resources )
      .add_resource( { props.matrix_buffer_name, matrix } )
      .add_resource( { props.staging_matrix_buffer_name, staging_matrix } )
      .add_resource( { props.write_request_buffer_name, write_request_buffer } )
  ) );
  read.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.read_shader )
      .set_swapchain_image_count( 1u )
      .set_external_descriptor_set( props.external_descriptor_set )
      .set_resources( props.resources )
      .add_resource( { props.matrix_buffer_name, matrix } )
      .add_resource( { props.staging_matrix_buffer_name, staging_matrix } )
      .add_resource( { props.read_request_buffer_name, read_request_buffer } )
  ) );
  update.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.update_shader )
      .set_swapchain_image_count( 1u )
      .set_external_descriptor_set( props.external_descriptor_set )
      .set_resources( props.resources )
      .add_resource( { props.matrix_buffer_name, matrix } )
      .add_resource( { props.update_request_buffer_name, update_request_buffer } )
  ) );
}

matrix_pool::matrix_descriptor matrix_pool::state_type::get_local( const matrix_descriptor &desc ) {
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return desc;
  }
  const auto &s = matrix_state[ *desc ];
  if( s.local ) {
    return s.local;
  }
  return desc;
}

std::vector< matrix_pool::request_range > matrix_pool::state_type::build_update_request_range() {
  std::vector< request_range > range;
  auto request = update_request_buffer->map< update_request >();
  std::uint32_t update_request_tail = 0u;
  for( unsigned int level = 0u; level != update_request_list.size(); ++level ) {
    if( !update_request_list[ level ].empty() ) {
      std::copy(
        update_request_list[ level ].begin(),
        update_request_list[ level ].end(),
        std::next( request.begin(), update_request_tail )
      );
      const std::uint32_t count = update_request_list[ level ].size();
      range.push_back(
        request_range()
          .set_offset( update_request_tail )
          .set_count( count )
      );
      update_request_tail += count;
    }
    else {
      range.push_back(
        request_range()
      );
    }
  }
  return range;
}

void matrix_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  {
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
  rec.compute_barrier( { matrix }, {} );
  auto update_range = build_update_request_range();
  for( unsigned int level = 0u; level != update_range.size(); ++level ) {
    if( !update_range[ level ].count ) {
      rec->pushConstants(
        **update->get_pipeline()->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( request_range ),
        reinterpret_cast< void* >( &update_range[ level ] )
      );
      (*update)( rec, 0u, update_range[ level ].count, 1u, 1u );
      rec.compute_barrier( { matrix }, {} );
    }
  }
  {
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
  rec.compute_barrier( { matrix }, {} );
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< std::function< void() > > cbs;
      std::vector< matrix_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        auto staging = self->staging_matrix->map< glm::mat4 >();
        for( const auto &desc: self->used_on_gpu ) {
          if( self->matrix_state.size() > *desc && self->matrix_state[ *desc ].valid ) {
            auto &s = self->matrix_state[ *desc ];
            if( s.read_request_index && s.staging_index ) {
              const auto corresponding = self->cbs.equal_range( *desc );
              if( corresponding.first != corresponding.second ) {
                if( result == vk::Result::eSuccess ) {
                  auto matrix = staging[ *s.staging_index ];
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result,matrix]() { cb( result, matrix ); } );
                  }
                }
                else {
                  for( auto iter = corresponding.first; iter != corresponding.second; ++iter ) {
                    cbs.push_back( [cb=iter->second,result]() { cb( result, glm::mat4() ); } );
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
        self->update_requested.clear();
        self->update_request_list.clear();
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

matrix_pool::matrix_pool( const matrix_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

matrix_pool::matrix_descriptor matrix_pool::allocate( const glm::mat4 &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( value );
}
matrix_pool::matrix_descriptor matrix_pool::allocate( const matrix_descriptor &parent, const glm::mat4 &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( parent, value );
}
void matrix_pool::touch( const matrix_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->touch( desc );
}
void matrix_pool::set( const matrix_descriptor &desc, const glm::mat4 &value ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->set( desc, value );
}
void matrix_pool::get( const matrix_descriptor &desc, const std::function< void( vk::Result, const glm::mat4& ) > &cb ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->get( desc, cb );
}
matrix_pool::matrix_descriptor matrix_pool::get_local( const matrix_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get_local( desc );
}
void matrix_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}
void matrix_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "matrix_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->matrix_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->matrix_state[ i ].staging_index ) {
        temp[ "staging_index" ] = *state->matrix_state[ i ].staging_index;
      }
      if( state->matrix_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->matrix_state[ i ].write_request_index;
      }
      if( state->matrix_state[ i ].read_request_index ) {
        temp[ "read_request_index" ] = *state->matrix_state[ i ].read_request_index;
      }
      temp[ "level" ] = state->matrix_state[ i ].level;
      if( state->matrix_state[ i ].local ) {
        temp[ "local" ] = *state->matrix_state[ i ].local;
      }
      if( state->matrix_state[ i ].parent ) {
        temp[ "parent" ] = *state->matrix_state[ i ].parent;
      }
      {
        auto d = state->matrix_state[ i ].self.lock();
        if( d ) {
          temp[ "self" ] = *d;
        }
      }
      dest[ "matrix_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "index_allocator" ] = state->index_allocator;
  dest[ "edge" ] = nlohmann::json::array();
  for( const auto &e: state->edge ) {
    auto temp = nlohmann::json::array();
    temp.push_back( e.first );
    temp.push_back( e.second );
    dest[ "edge" ].push_back( temp );
  }
  dest[ "staging_matrix" ] = nlohmann::json::array();
  {
    auto m = state->staging_matrix->map< glm::mat4 >();
    for( std::uint32_t i = 0u; i != state->staging_index_allocator.get_tail(); ++i ) {
      auto e = m[ i ];
      auto temp = nlohmann::json::array();
      {
        temp.push_back( e[ 0 ][ 0 ] );
        temp.push_back( e[ 0 ][ 1 ] );
        temp.push_back( e[ 0 ][ 2 ] );
        temp.push_back( e[ 0 ][ 3 ] );
        temp.push_back( e[ 1 ][ 0 ] );
        temp.push_back( e[ 1 ][ 1 ] );
        temp.push_back( e[ 1 ][ 2 ] );
        temp.push_back( e[ 1 ][ 3 ] );
        temp.push_back( e[ 2 ][ 0 ] );
        temp.push_back( e[ 2 ][ 1 ] );
        temp.push_back( e[ 2 ][ 2 ] );
        temp.push_back( e[ 2 ][ 3 ] );
        temp.push_back( e[ 3 ][ 0 ] );
        temp.push_back( e[ 3 ][ 1 ] );
        temp.push_back( e[ 3 ][ 2 ] );
        temp.push_back( e[ 3 ][ 3 ] );
      }
      dest[ "staging_matrix" ].push_back( temp );
    }
  }
  dest[ "matrix" ] = nlohmann::json::object();
  {
    auto m = state->matrix->map< glm::mat4 >();
    for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
      if( state->matrix_state[ i ].valid ) {
        auto e = m[ i ];
        auto temp = nlohmann::json::array();
        {
          temp.push_back( e[ 0 ][ 0 ] );
          temp.push_back( e[ 0 ][ 1 ] );
          temp.push_back( e[ 0 ][ 2 ] );
          temp.push_back( e[ 0 ][ 3 ] );
          temp.push_back( e[ 1 ][ 0 ] );
          temp.push_back( e[ 1 ][ 1 ] );
          temp.push_back( e[ 1 ][ 2 ] );
          temp.push_back( e[ 1 ][ 3 ] );
          temp.push_back( e[ 2 ][ 0 ] );
          temp.push_back( e[ 2 ][ 1 ] );
          temp.push_back( e[ 2 ][ 2 ] );
          temp.push_back( e[ 2 ][ 3 ] );
          temp.push_back( e[ 3 ][ 0 ] );
          temp.push_back( e[ 3 ][ 1 ] );
          temp.push_back( e[ 3 ][ 2 ] );
          temp.push_back( e[ 3 ][ 3 ] );
        }
        dest[ "matrix" ][ std::to_string( i ) ] = temp;
      }
    }
  }
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
  dest[ "update_request_list" ] = nlohmann::json::array();
  for( const auto &level: state->update_request_list ) {
    auto l = nlohmann::json::array();
    for( auto e: level ) {
      auto temp = nlohmann::json::object();
      temp[ "parent" ] = e.parent;
      temp[ "local" ] = e.local;
      temp[ "world" ] = e.world;
      l.push_back( temp );
    }
    dest[ "update_request_list" ].push_back( l );
  }
  dest[ "update_requested" ] = nlohmann::json::array();
  for( auto e: state->update_requested ) {
    dest[ "update_requested" ].push_back( e );
  }
  dest[ "staging_index_allocator" ] = state->staging_index_allocator;
  dest[ "write_request_index_allocator" ] = state->write_request_index_allocator;
  dest[ "read_request_index_allocator" ] = state->read_request_index_allocator;
  dest[ "used_on_gpu" ] = nlohmann::json::array();
  for( const auto &e: state->used_on_gpu ) {
    dest[ "used_on_gpu" ].push_back( *e );
  }
  dest[ "modified" ] = nlohmann::json::array();
  for( auto e: state->modified ) {
    dest[ "modified" ].push_back( e );
  }
  dest[ "write" ] = *state->write;
  dest[ "read" ] = *state->read;
  dest[ "update" ] = *state->update;
  dest[ "execution_pending" ] = state->execution_pending;
}
void to_json( nlohmann::json &dest, const matrix_pool &src ) {
  src.to_json( dest );
}

}

