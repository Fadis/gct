#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/exception.hpp>
#include <gct/buffer.hpp>

namespace gct {
  void command_buffer_recorder_t::bind_vertex_buffer(
    std::shared_ptr< buffer_t > vertex_buffer
  ) {
    (*get_factory())->bindVertexBuffers( 0, { **vertex_buffer }, { 0 } );
    get_factory()->unbound()->keep.push_back( vertex_buffer );
  }
  void command_buffer_recorder_t::bind_vertex_buffer(
    std::shared_ptr< buffer_t > vertex_buffer,
    vk::DeviceSize offset
  ) {
    (*get_factory())->bindVertexBuffers( 0, { **vertex_buffer }, { offset } );
    get_factory()->unbound()->keep.push_back( vertex_buffer );
  }
  void command_buffer_recorder_t::bind_index_buffer(
    std::shared_ptr< buffer_t > index_buffer,
    vk::IndexType type
  ) {
    (*get_factory())->bindIndexBuffer( **index_buffer, 0, type );
    get_factory()->unbound()->keep.push_back( index_buffer );
  }
  void command_buffer_recorder_t::bind_index_buffer(
    std::shared_ptr< buffer_t > index_buffer,
    vk::DeviceSize offset,
    vk::IndexType type
  ) {
    (*get_factory())->bindIndexBuffer( **index_buffer, offset, type );
    get_factory()->unbound()->keep.push_back( index_buffer );
  }
#if defined(VK_VERSION_1_3) || defined(VK_EXT_EXTENDED_DYNAMIC_STATE_EXTENSION_NAME) || defined(VK_EXT_SHADER_OBJECT_EXTENSION_NAME)
  void command_buffer_recorder_t::bind_vertex_buffer(
    std::uint32_t first_binding,
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< vk::DeviceSize > &offset,
    const std::vector< vk::DeviceSize > &size,
    const std::vector< vk::DeviceSize > &stride
  ) {
    if( buffer.size() != offset.size() ) {
      throw exception::invalid_argument( "command_buffer_recorder_t::bind_vertex_buffer : buffer.size() != offset.size()" );
    }
    if( buffer.size() != size.size() ) {
      throw exception::invalid_argument( "command_buffer_recorder_t::bind_vertex_buffer : buffer.size() != size.size()" );
    }
    if( buffer.size() != stride.size() ) {
      throw exception::invalid_argument( "command_buffer_recorder_t::bind_vertex_buffer : buffer.size() != stride.size()" );
    }
    std::vector< vk::Buffer > temp;
    temp.reserve( buffer.size() );
    std::transform(
      buffer.begin(),
      buffer.end(),
      std::back_inserter( temp ),
      []( const auto &e ) {
        return **e;
      }
    );
#ifdef VK_VERSION_1_3
    (*get_factory())->bindVertexBuffers2(
#else
    (*get_factory())->bindVertexBuffers2EXT(
#endif
      first_binding,
      temp.size(),
      temp.data(),
      offset.data(),
      size.data(),
      stride.data()
    );
    get_factory()->unbound()->keep.push_back( buffer );
  }
  void command_buffer_recorder_t::bind_vertex_buffer(
    std::uint32_t first_binding,
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< vk::DeviceSize > &size,
    const std::vector< vk::DeviceSize > &stride
  ) {
    std::vector< vk::DeviceSize > offset( buffer.size(), vk::DeviceSize( 0 ) );
    bind_vertex_buffer(
      first_binding,
      buffer,
      offset,
      size,
      stride
    );
  }
  void command_buffer_recorder_t::bind_vertex_buffer(
    std::uint32_t first_binding,
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< vk::DeviceSize > &stride
  ) {
    std::vector< vk::DeviceSize > size;
    size.reserve( buffer.size() );
    std::transform(
      buffer.begin(),
      buffer.end(),
      std::back_inserter( size ),
      []( const auto &e ) {
        return e->get_props().get_basic().size;
      }
    );
    bind_vertex_buffer(
      first_binding,
      buffer,
      size,
      stride
    );
  }
  void command_buffer_recorder_t::bind_vertex_buffer(
    const std::vector< std::shared_ptr< buffer_t > > &buffer,
    const std::vector< vk::DeviceSize > &stride
  ) {
    bind_vertex_buffer(
      0u,
      buffer,
      stride
    );
  }
#endif
}

