#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>
#include <gct/buffer.hpp>

namespace gct {
  void command_buffer_recorder_t::bind_vertex_buffer(
    std::shared_ptr< buffer_t > vertex_buffer
  ) {
    (*get_factory())->bindVertexBuffers( 0, { **vertex_buffer }, { 0 } );
    get_factory()->unbound()->keep.push_back( vertex_buffer );
  }
}

