#include <gct/command_buffer_recorder.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>
#include <gct/image_layout.hpp>
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
      [command_buffer=get_factory(),begin_info]( void* ) {
        (*command_buffer)->endRenderPass();
        const auto fb = begin_info.get_framebuffer();
        const auto rp = begin_info.get_render_pass();
        if( fb && rp ) {
          const auto ra = rp->get_props().get_attachment();
          const auto fa = fb->get_props().get_attachment();
          if( ra.size() == fa.size() ) {
            for( unsigned int i = 0u; i != ra.size(); ++i ) {
              fa[ i ]->get_factory()->get_layout().set_layout(
                fa[ i ]->get_props().get_basic().subresourceRange.baseMipLevel,
                fa[ i ]->get_props().get_basic().subresourceRange.levelCount,
                fa[ i ]->get_props().get_basic().subresourceRange.baseArrayLayer,
                fa[ i ]->get_props().get_basic().subresourceRange.layerCount,
                ra[ i ].finalLayout
              );
            }
          }
        }
      }
    );
  }
}
