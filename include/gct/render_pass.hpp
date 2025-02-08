#ifndef LIBGCT_INCLUDE_GCT_RENDER_PASS_HPP
#define LIBGCT_INCLUDE_GCT_RENDER_PASS_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/render_pass_create_info.hpp>
#include <gct/framebuffer.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  class device_t;
  class framebuffer_create_info_t;
  class render_pass_t :
    public vulkan_handle< vk::RenderPass >,
    public property< render_pass_create_info_t >,
    public created_from< device_t >,
    public std::enable_shared_from_this< render_pass_t > {
  public:
    render_pass_t(
      const std::shared_ptr< device_t >&,
      const render_pass_create_info_t&
    );
    [[nodiscard]] std::shared_ptr< framebuffer_t > get_framebuffer(
      const framebuffer_create_info_t&
    );
  };

}

#endif
