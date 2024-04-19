#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/device.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
void command_buffer_recorder_t::fill(
  const std::shared_ptr< buffer_t > &dest,
  std::uint32_t value
) {
  (*get_factory())->fillBuffer(
    **dest,
    0u,
    dest->get_props().get_basic().size,
    value
  );
  get_factory()->unbound()->keep.push_back( dest );
}
void command_buffer_recorder_t::fill(
  const std::shared_ptr< mappable_buffer_t > &dest,
  std::uint32_t value
) {
  fill( dest->get_buffer(), value );
}
}

