#include <vulkan/vulkan.hpp>
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
#include <gct/device.hpp>
#include <gct/render_pass2.hpp>

namespace gct {
  render_pass2_t::render_pass2_t(
    const std::shared_ptr< device_t > &device,
    const render_pass_create_info2_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
#ifdef VK_VERSION_1_2
    handle = (*device)->createRenderPass2Unique(
#else
    handle = (*device)->createRenderPass2KHRUnique(
#endif
      props.get_basic()
    );
  }
  std::shared_ptr< framebuffer_t > render_pass2_t::get_framebuffer(
    const framebuffer_create_info_t &create_info
  ) {
    return std::shared_ptr< framebuffer_t >(
      new framebuffer_t(
        shared_from_this(),
        create_info
      )
    );
  }
}

#endif

