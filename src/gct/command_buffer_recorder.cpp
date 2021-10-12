#include <gct/buffer.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>

namespace gct {
  command_buffer_recorder_t::command_buffer_recorder_t(
    const std::shared_ptr< bound_command_buffer_t > &cb,
    const command_buffer_begin_info_t &begin_info
  ) :
    created_from< bound_command_buffer_t >( cb ),
    props( begin_info ) {
    props.rebuild_chain();
    get_factory()->reset();
    (*get_factory())->begin( props.get_basic() );
  }
  command_buffer_recorder_t::~command_buffer_recorder_t() {
    (*get_factory())->end();
  }
  vk::CommandBuffer &command_buffer_recorder_t::operator*() {
    return **get_factory();
  }
  const vk::CommandBuffer &command_buffer_recorder_t::operator*() const {
    return **get_factory();
  }
  vk::CommandBuffer *command_buffer_recorder_t::operator->() {
    return &**get_factory();
  }
  const vk::CommandBuffer *command_buffer_recorder_t::operator->() const {
    return &**get_factory();
  }
}
