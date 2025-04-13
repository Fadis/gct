#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/format.hpp>
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/compute.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/command_buffer.hpp>
#include <gct/exception.hpp>
#include <gct/vertex_buffer_pool.hpp>
namespace gct {

vertex_buffer_pool::vertex_buffer_index_t vertex_buffer_pool::state_type::allocate_index() {
  const auto index = index_allocator.allocate();
  if( index >= vertex_buffer_state.size() ) {
    vertex_buffer_state.resize( index + 1u );
  }
  return index; 
}

void vertex_buffer_pool::state_type::release_index( vertex_buffer_pool::vertex_buffer_index_t index ) {
  if( index >= vertex_buffer_state.size() || !vertex_buffer_state[ index ].valid ) {
    return;
  }
  vertex_buffer_state[ index ].valid = false;
  index_allocator.release( index );
}

vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_pool::state_type::allocate(
  const std::string &filename
) {
  if( execution_pending ) {
    throw exception::runtime_error( "vertex_buffer_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  std::ifstream file( filename, std::ios::in | std::ios::binary );
  std::vector< std::uint8_t > data(
    ( std::istreambuf_iterator< char >( file ) ),
    std::istreambuf_iterator< char >()
  );
    
  auto b = props.allocator_set.allocator->create_mappable_buffer(
    data.size(),
    vk::BufferUsageFlagBits::eVertexBuffer|
    vk::BufferUsageFlagBits::eIndexBuffer|
    vk::BufferUsageFlagBits::eStorageBuffer
  );
  {
    auto mapped = b->map< std::uint8_t >();
    std::copy( data.begin(), data.end(), mapped.begin() );
  }

  const vertex_buffer_index_t index = allocate_index();

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_buffer( b )
  );
  vertex_buffer_state[ index ] =
    vertex_buffer_state_type()
      .set_valid( true )
      .set_buffer( b )
      .set_write_request_index( write_request_list.size() - 1u );

  vertex_buffer_descriptor desc(
    new vertex_buffer_index_t( index ),
    [self=shared_from_this()]( const vertex_buffer_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  return desc;
}

vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_pool::state_type::allocate(
  const std::vector< glm::vec4 > &data
) {
  if( execution_pending ) {
    throw exception::runtime_error( "vertex_buffer_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  auto b = props.allocator_set.allocator->create_mappable_buffer(
    data.size() * sizeof( glm::vec4 ),
    vk::BufferUsageFlagBits::eVertexBuffer|
    vk::BufferUsageFlagBits::eIndexBuffer|
    vk::BufferUsageFlagBits::eStorageBuffer
  );
  {
    auto mapped = b->map< std::uint8_t >();
    std::copy(
      reinterpret_cast< const std::uint8_t* >( data.data() ),
      reinterpret_cast< const std::uint8_t* >( std::next( data.data(), data.size() ) ),
      mapped.begin()
    );
  }
  const vertex_buffer_index_t index = allocate_index();

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_buffer( b )
  );
  vertex_buffer_state[ index ] =
    vertex_buffer_state_type()
      .set_valid( true )
      .set_buffer( b )
      .set_write_request_index( write_request_list.size() - 1u );

  vertex_buffer_descriptor desc(
    new vertex_buffer_index_t( index ),
    [self=shared_from_this()]( const vertex_buffer_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  return desc;
}

vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_pool::state_type::allocate(
  const std::vector< std::uint8_t > &data
) {
  if( execution_pending ) {
    throw exception::runtime_error( "vertex_buffer_pool::state_type::allocate : last execution is not completed yet", __FILE__, __LINE__ );
  }

  auto b = props.allocator_set.allocator->create_mappable_buffer(
    data.size(),
    vk::BufferUsageFlagBits::eVertexBuffer|
    vk::BufferUsageFlagBits::eIndexBuffer|
    vk::BufferUsageFlagBits::eStorageBuffer
  );
  {
    auto mapped = b->map< std::uint8_t >();
    std::copy(
      data.data(),
      std::next( data.data(), data.size() ),
      mapped.begin()
    );
  }
  const vertex_buffer_index_t index = allocate_index();

  write_request_list.push_back(
    write_request()
      .set_index( index )
      .set_buffer( b )
  );
  vertex_buffer_state[ index ] =
    vertex_buffer_state_type()
      .set_valid( true )
      .set_buffer( b )
      .set_write_request_index( write_request_list.size() - 1u );

  vertex_buffer_descriptor desc(
    new vertex_buffer_index_t( index ),
    [self=shared_from_this()]( const vertex_buffer_index_t *p ) {
      if( p ) {
        self->release( *p );
        delete p;
      }
    }
  );
  used_on_gpu.push_back( desc );
  return desc;
}

void vertex_buffer_pool::state_type::release( vertex_buffer_index_t index ) {
  vertex_buffer_state_type removed;
  {
    std::lock_guard< std::mutex > lock( guard );
    if( vertex_buffer_state.size() <= index || !vertex_buffer_state[ index ].valid ) {
      return;
    }
    removed = std::move( vertex_buffer_state[ index ] );
    vertex_buffer_state[ index ] = vertex_buffer_state_type();
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
          .add_buffer( null_buffer )
      );
      props.descriptor_set->update(
        std::move( updates )
      );
    };*/
  }
}


std::shared_ptr< buffer_t > vertex_buffer_pool::state_type::get( const vertex_buffer_descriptor &desc ) {
  if( vertex_buffer_state.size() <= *desc || !vertex_buffer_state[ *desc ].valid ) {
    throw exception::invalid_argument( "vertex_buffer_pool::get : No such sampler" );
  }
  return vertex_buffer_state[ *desc ].buffer->get_buffer();
}

std::vector< std::shared_ptr< buffer_t > > vertex_buffer_pool::state_type::get() {
  std::vector< std::shared_ptr< buffer_t > > temp;
  temp.reserve( vertex_buffer_state.size() );
  for( auto &e: vertex_buffer_state ) {
    temp.push_back( e.buffer->get_buffer() );
  }
  return temp;
}

vertex_buffer_pool::state_type::state_type( const vertex_buffer_pool_create_info &ci ) :
  props( ci ),
  index_allocator( linear_allocator_create_info().set_max( ci.max_vertex_buffer_count ) ) {
}

void vertex_buffer_pool::state_type::flush( command_buffer_recorder_t &rec ) {
  if( execution_pending ) {
    return;
  }
  if( props.external_descriptor_set.find( props.vertex_buffer_descriptor_set_id ) != props.external_descriptor_set.end() ) {
    std::vector< write_descriptor_set_t > updates;
    for( const auto &req: write_request_list ) {
      rec.sync_to_device( req.buffer );
      const auto target = (*props.external_descriptor_set[ props.vertex_buffer_descriptor_set_id ])[ props.descriptor_name ];
      updates.push_back(
        write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet( target )
              .setDstArrayElement( req.index )
              .setDescriptorCount( 1u )
          )
          .add_buffer( req.buffer->get_buffer() )
          .set_index( req.index )
      );
    }
    props.external_descriptor_set[ props.vertex_buffer_descriptor_set_id ]->update(
      std::move( updates )
    );
  }
  else {
    for( const auto &req: write_request_list ) {
      rec.sync_to_device( req.buffer );
    }
  }
  rec.on_executed(
    [self=shared_from_this()]( vk::Result result ) {
      std::vector< vertex_buffer_descriptor > used_on_gpu;
      {
        std::lock_guard< std::mutex > lock( self->guard );
        for( const auto &desc: self->used_on_gpu ) {
          if( self->vertex_buffer_state.size() > *desc && self->vertex_buffer_state[ *desc ].valid ) {
            auto &s = self->vertex_buffer_state[ *desc ];
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

vertex_buffer_pool::vertex_buffer_pool( const vertex_buffer_pool_create_info &ci ) :
  state( new state_type( ci ) ) {
}

vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_pool::allocate(
  const std::string &filename
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( filename );
}

vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_pool::allocate(
  const std::vector< glm::vec4 > &data
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( data );
}

vertex_buffer_pool::vertex_buffer_descriptor vertex_buffer_pool::allocate(
  const std::vector< std::uint8_t > &data
) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->allocate( data );
}

std::shared_ptr< buffer_t > vertex_buffer_pool::get( const vertex_buffer_descriptor &desc ) {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get( desc );
}

std::vector< std::shared_ptr< buffer_t > > vertex_buffer_pool::get() {
  std::lock_guard< std::mutex > lock( state->guard );
  return state->get();
}

void vertex_buffer_pool::operator()( command_buffer_recorder_t &rec ) {
  std::lock_guard< std::mutex > lock( state->guard );
  state->flush( rec );
}

void vertex_buffer_pool::to_json( nlohmann::json &dest ) const {
  std::lock_guard< std::mutex > lock( state->guard );
  dest = nlohmann::json::object();
  dest[ "props" ] = get_props();
  dest[ "vertex_buffer_state" ] = nlohmann::json::object();
  for( std::uint32_t i = 0u; i != state->index_allocator.get_tail(); ++i ) {
    if( state->vertex_buffer_state[ i ].valid ) {
      auto temp = nlohmann::json::object();
      if( state->vertex_buffer_state[ i ].write_request_index ) {
        temp[ "write_request_index" ] = *state->vertex_buffer_state[ i ].write_request_index;
      }
      if( state->vertex_buffer_state[ i ].buffer ) {
        temp[ "buffer" ] = *state->vertex_buffer_state[ i ].buffer;
      }
      dest[ "vertex_buffer_state" ][ std::to_string( i ) ] = temp;
    }
  }
  dest[ "write_request_buffer" ] = nlohmann::json::array();
  for( const auto &w: state->write_request_list ) {
    auto temp = nlohmann::json::object();
    temp[ "index" ] = w.index;
    temp[ "buffer" ] = *w.buffer;
    dest[ "write_request_buffer" ].push_back( temp );
  }
  dest[ "execution_pending" ] = state->execution_pending;
}

void to_json( nlohmann::json &dest, const vertex_buffer_pool &src ) {
  src.to_json( dest );
}

}

