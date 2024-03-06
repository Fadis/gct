#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/compute.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/matrix_pool.hpp>
namespace gct {

matrix_index_t matrix_pool::state_type::allocate_index() {
  matrix_index_t index = 0u;
  if( !deallocated.empty() ) {
    index = deallocated.back();
    deallocated.pop_back();
  }
  else {
    index = matrix_state.size();
    matrix_state.resize( matrix_state.size() + 1u );
  }
  return index; 
}

matrix_index_t matrix_pool::state_type::allocate_staging_index() {
  matrix_index_t staging_index = staging_tail;
  ++staging_tail;
  return staging_index;
}

request_index_t matrix_pool::state_type::allocate_write_request_index() {
  request_index_t write_request_index = write_request_tail;
  ++write_request_tail;
  return write_request_index;
}

request_index_t matrix_pool::state_type::allocate_read_request_index() {
  request_index_t read_request_index = read_request_tail;
  ++read_request_tail;
  return read_request_index;
}

void matrix_pool::state_type::release_index( matrix_index_t index ) {
  if( index >= matrix_state.size() || !matrix_state[ index ].valid ) {
    return;
  }
  matrix_state[ index ].valid = false;
  deallocated.push_back( index );
}

matrix_descriptor matrix_pool::state_type::allocate( const glm::mat4 &value, bool auto_touch ) {
  const matrix_index_t index = allocate_index();

  const matrix_index_t staging_index = allocate_staging_index();

  const request_index_t write_request_index = allocate_write_request_index();

  write_request_buffer->map< write_request >()[ write_request_index ] =
    write_request()
      .set_staging( staging_index )
      .set_destination( index );

  staging_matrix->map< glm::mat4 >()[ staging_index ] = value;

  matrix_state[ index ] =
    matrix_state_type()
      .set_valid( true )
      .set_auto_touch( auto_touch )
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
  matrix_state[ index ].set_self( desc.get_weak() );
  used_on_gpu.push_back( desc );
  return desc;
}
matrix_descriptor matrix_pool::state_type::allocate( const matrix_descriptor &parent, const glm::mat4 &value, bool auto_touch ) {
  if( *parent >= matrix_state.size() || !matrix_state[ *parent ].valid ) {
    throw -1;
  }
  
  const std::uint32_t level = matrix_state[ *parent ].level + 1u; 
  if( update_request_list.size() <= level ) {
    update_request_list.resize( level + 1u );
  }

  const matrix_index_t index = allocate_index();
  const matrix_index_t local_index = allocate_index();
  const matrix_index_t staging_index = allocate_staging_index();
  const request_index_t write_request_index = allocate_write_request_index();

  write_request_buffer->map< write_request >()[ write_request_index ] =
    write_request()
      .set_staging( staging_index )
      .set_destination( local_index );

  staging_matrix->map< glm::mat4 >()[ staging_index ] = value;

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

  matrix_state[ index ] =
    matrix_state_type()
      .set_valid( true )
      .set_auto_touch( auto_touch )
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
  matrix_state[ local_index ].set_self( local_desc.get_weak() );
  matrix_state[ index ].set_self( desc.get_weak() );
  used_on_gpu.push_back( local_desc );
  used_on_gpu.push_back( desc );
  return desc;
}

void matrix_pool::state_type::release( matrix_index_t index ) {
  if( matrix_state.size() <= index || !matrix_state[ index ].valid ) {
    return;
  }
  matrix_state[ index ] = matrix_state_type();
  release_index( index );
}

void matrix_pool::state_type::touch( const matrix_descriptor &desc ) {
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
    const auto &l = matrix_state[ *s.local ];
    used_on_gpu.push_back( l.self );
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
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return;
  }
  auto &s = matrix_state[ *desc ];
  if( s.local ) {
    auto &l = matrix_state[ *s.local ];
    if( l.staging_index && l.write_request_index ) {
      staging_matrix->map< glm::mat4 >()[ *l.staging_index ] = value;
    }
    else if( l.staging_index ) {
      staging_matrix->map< glm::mat4 >()[ *l.staging_index ] = value;
      const request_index_t write_request_index = allocate_write_request_index();
      write_request_buffer->map< write_request >()[ write_request_index ] =
        write_request()
          .set_staging( *l.staging_index )
          .set_destination( *s.local );
      l.set_write_request_index( write_request_index );
    }
    else {
      const matrix_index_t staging_index = allocate_staging_index();
      staging_matrix->map< glm::mat4 >()[ staging_index ] = value;
      const request_index_t write_request_index = allocate_write_request_index();
      write_request_buffer->map< write_request >()[ write_request_index ] =
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
      staging_matrix->map< glm::mat4 >()[ *s.staging_index ] = value;
    }
    else if( s.staging_index ) {
      staging_matrix->map< glm::mat4 >()[ *s.staging_index ] = value;
      const request_index_t write_request_index = allocate_write_request_index();
      write_request_buffer->map< write_request >()[ write_request_index ] =
        write_request()
          .set_staging( *s.staging_index )
          .set_destination( *desc );
      s.set_write_request_index( write_request_index );
    }
    else {
      const matrix_index_t staging_index = allocate_staging_index();
      staging_matrix->map< glm::mat4 >()[ staging_index ] = value;
      const request_index_t write_request_index = allocate_write_request_index();
      write_request_buffer->map< write_request >()[ write_request_index ] =
        write_request()
          .set_staging( staging_index )
          .set_destination( *desc );
      s.set_write_request_index( write_request_index );
      s.set_staging_index( staging_index );
      used_on_gpu.push_back( desc );
    }
  }
  touch( desc );
}

void matrix_pool::state_type::get( const matrix_descriptor &desc, const std::function< void( const glm::mat4& ) > &cb ) {
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return;
  }
  auto &s = matrix_state[ *desc ];
  if( s.staging_index && s.read_request_index ) {
    cbs.insert( std::make_pair( *desc, cb ) );
  }
  else if( s.staging_index ) {
    cbs.insert( std::make_pair( *desc, cb ) );
    const request_index_t read_request_index = allocate_read_request_index();
    read_request_buffer->map< read_request >()[ read_request_index ] =
      read_request()
        .set_source( *desc )
        .set_staging( *s.staging_index );
    s.set_read_request_index( read_request_index );
  }
  else {
    cbs.insert( std::make_pair( *desc, cb ) );
    const matrix_index_t staging_index = allocate_staging_index();
    const request_index_t read_request_index = allocate_read_request_index();
    read_request_buffer->map< read_request >()[ read_request_index ] =
      read_request()
        .set_source( *desc )
        .set_staging( staging_index );
    s.set_read_request_index( read_request_index );
    s.set_staging_index( staging_index );
    used_on_gpu.push_back( desc );
  }
}

matrix_pool::state_type::state_type( const matrix_pool_create_info &ci ) :
  props( ci ) {
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
      .set_resources( props.resources )
      .add_resource( { props.matrix_buffer_name, matrix } )
      .add_resource( { props.update_request_buffer_name, update_request_buffer } )
  ) );
}

matrix_descriptor matrix_pool::state_type::get_local( const matrix_descriptor &desc ) {
  if( matrix_state.size() <= *desc || !matrix_state[ *desc ].valid ) {
    return desc;
  }
  if( update_requested.find( *desc ) != update_requested.end() ) {
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
  (*write)( rec, 0u, write_request_tail, 1u, 1u );
  rec.compute_barrier( { matrix }, {} );
  auto update_range = build_update_request_range();
  for( unsigned int level = 0u; level != update_range.size(); ++level ) {
    if( !update_range[ level ].count ) {
      rec->pushConstants(
        **update->get_pipeline()->get_props().get_layout(),
        vk::ShaderStageFlagBits::eCompute,
        0u,
        sizeof( update_range ),
        reinterpret_cast< void* >( &update_range[ level ] )
      );
      (*update)( rec, 0u, update_range[ level ].count, 1u, 1u );
      rec.compute_barrier( { matrix }, {} );
    }
  }
  (*read)( rec, 0u, read_request_tail, 1u, 1u );
  ///////
}

matrix_pool::matrix_pool( const matrix_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

matrix_descriptor matrix_pool::allocate( const glm::mat4 &value, bool auto_touch ) {
  return state->allocate( value, auto_touch );
}
matrix_descriptor matrix_pool::allocate( const matrix_descriptor &parent, const glm::mat4 &value, bool auto_touch ) {
  return state->allocate( parent, value, auto_touch );
}
void matrix_pool::touch( const matrix_descriptor &desc ) {
  state->touch( desc );
}
void matrix_pool::set( const matrix_descriptor &desc, const glm::mat4 &value ) {
  state->set( desc, value );
}
void matrix_pool::get( const matrix_descriptor &desc, const std::function< void( const glm::mat4& ) > &cb ) {
  state->get( desc, cb );
}
matrix_descriptor matrix_pool::get_local( const matrix_descriptor &desc ) {
  return state->get_local( desc );
}
void matrix_pool::operator()( command_buffer_recorder_t &rec ) {
  state->flush( rec );
}

}

