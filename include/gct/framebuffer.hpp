#ifndef GCT_FRAMEBUFFER_HPP
#define GCT_FRAMEBUFFER_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/framebuffer_create_info.hpp>

namespace gct {
  class device_t;
  class render_pass_t;
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
  class render_pass2_t;
#endif
  class framebuffer_t : public created_from< device_t > {
  public:
    framebuffer_t(
      const std::shared_ptr< render_pass_t >&,
      const framebuffer_create_info_t&
    );
#if defined(VK_VERSION_1_2) || defined(VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME)
    framebuffer_t(
      const std::shared_ptr< render_pass2_t >&,
      const framebuffer_create_info_t&
    );
#endif
    framebuffer_t( const framebuffer_t& ) = delete;
    framebuffer_t( framebuffer_t&& ) = default;
    framebuffer_t &operator=( const framebuffer_t& ) = delete;
    framebuffer_t &operator=( framebuffer_t&& ) = default;
    const framebuffer_create_info_t &get_props() const { return props; }
    vk::Framebuffer &operator*() {
      return *handle;
    }
    const vk::Framebuffer &operator*() const {
      return *handle;
    }
    vk::Framebuffer *operator->() {
      return &handle.get();
    }
    const vk::Framebuffer *operator->() const {
      return &handle.get();
    }
  private:
    framebuffer_create_info_t props;
    vk::UniqueHandle< vk::Framebuffer, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

