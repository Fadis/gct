#ifndef LIBGCT_INCLUDE_GCT_RENDER_PASS2_HPP
#define LIBGCT_INCLUDE_GCT_RENDER_PASS2_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/render_pass_create_info2.hpp>
#include <gct/framebuffer.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
namespace gct {
  class device_t;
  class framebuffer_create_info2_t;
  class render_pass2_t :
    public vulkan_handle< vk::RenderPass >,
    public property< render_pass_create_info2_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< render_pass2_t > {
  public:
    render_pass2_t(
      const std::shared_ptr< device_t >&,
      const render_pass_create_info2_t&
    );
    [[nodiscard]] std::shared_ptr< framebuffer_t > get_framebuffer(
      const framebuffer_create_info_t&
    );
  };

}
#endif

#endif
