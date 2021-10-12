#ifndef LIBGCT_INCLUDE_GCT_RENDER_PASS_HPP
#define LIBGCT_INCLUDE_GCT_RENDER_PASS_HPP

#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/render_pass_create_info.hpp>
#include <gct/framebuffer.hpp>

namespace gct {
  class device_t;
  class framebuffer_create_info_t;
  class render_pass_t : public created_from< device_t >, public std::enable_shared_from_this< render_pass_t > {
  public:
    render_pass_t(
      const std::shared_ptr< device_t >&,
      const render_pass_create_info_t&
    );
    vk::RenderPass &operator*() {
      return *handle;
    }
    const vk::RenderPass &operator*() const {
      return *handle;
    }
    vk::RenderPass* operator->() {
      return &handle.get();
    }
    const vk::RenderPass* operator->() const {
      return &handle.get();
    }
    const render_pass_create_info_t &get_props() const {
      return props;
    }
    std::shared_ptr< framebuffer_t > get_framebuffer(
      const framebuffer_create_info_t&
    );
  private:
    render_pass_create_info_t props;
    vk::UniqueHandle< vk::RenderPass, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };

}

#endif
