#include <gct/render_pass.hpp>
#include <gct/framebuffer.hpp>
#include <gct/allocator.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/voxel_framebuffer.hpp>

namespace gct {
voxel_framebuffer::voxel_framebuffer(
  const std::shared_ptr< allocator_t > &allocator,
  unsigned int size_factor
) {
  const std::uint32_t size = 1u << size_factor;
  render_pass = get_device( *allocator ).get_render_pass(
    gct::render_pass_create_info_t()
      .add_subpass(
        gct::subpass_description_t()
      )
      .rebuild_chain()
  );
  framebuffer = render_pass->get_framebuffer(
    gct::framebuffer_create_info_t()
      .set_basic(
        vk::FramebufferCreateInfo()
          .setWidth( 1 << size )
          .setHeight( 1 << size )
          .setLayers( 1 )
      )
  );
  rpbi = gct::render_pass_begin_info_t()
    .set_render_pass( render_pass )
    .set_framebuffer( framebuffer );
  viewport =
    vk::Viewport()
      .setWidth( 1 << size )
      .setHeight( 1 << size )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  scissor =
    vk::Rect2D( vk::Offset2D(0, 0), vk::Extent2D( 1 << size, 1 << size ) );
}

}

