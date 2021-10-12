#ifndef GCT_FRAMEBUFFER_HPP
#define GCT_FRAMEBUFFER_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/framebuffer_create_info.hpp>

namespace gct {
  struct render_pass_t;
  class framebuffer_t : public created_from< render_pass_t > {
  public:
    framebuffer_t(
      const std::shared_ptr< render_pass_t >&,
      const framebuffer_create_info_t&
    );
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

