#include <gct/command_buffer_recorder.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/command_buffer.hpp>
namespace gct {
  std::shared_ptr< void > command_buffer_recorder_t::begin_render_pass(
    const render_pass_begin_info_t &begin_info,
    vk::SubpassContents subpass_contents
  ) {
    auto temp = begin_info;
    temp.rebuild_chain();
    (*get_factory())->beginRenderPass(
      temp.get_basic(), subpass_contents
    );
    return std::shared_ptr< void >(
      nullptr,
      [command_buffer=get_factory()]( void* ) {
        (*command_buffer)->endRenderPass();
      }
    );
  }
}
