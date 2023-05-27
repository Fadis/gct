#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
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
}

