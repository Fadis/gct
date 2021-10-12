#ifndef GCT_SWAPCHAIN_IMAGE_HPP
#define GCT_SWAPCHAIN_IMAGE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>

namespace gct {
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  image_create_info_t to_image_create_info( const vk::SwapchainCreateInfoKHR& );
  class swapchain_t;
  class swapchain_image_t : public image_t, public created_from< swapchain_t >, public std::enable_shared_from_this< swapchain_image_t > {
  public:
    swapchain_image_t(
      const std::shared_ptr< swapchain_t >&,
      const vk::Image&
    );
    swapchain_image_t( const swapchain_image_t& ) = delete;
    swapchain_image_t( swapchain_image_t&& ) = default;
    swapchain_image_t &operator=( const swapchain_image_t& ) = delete;
    swapchain_image_t &operator=( swapchain_image_t&& ) = default;
    const image_create_info_t &get_props() const { return props; }
    std::shared_ptr< image_view_t > get_view(
      const image_view_create_info_t&
    );
    std::shared_ptr< image_view_t > get_view(
      vk::ImageAspectFlags aspect
    );
    vk::Image &operator*() {
      return handle;
    }
    const vk::Image &operator*() const {
      return handle;
    }
    vk::Image *operator->() {
      return &handle;
    }
    const vk::Image *operator->() const {
      return &handle;
    }
    std::shared_ptr< device_t > get_device() const;
  private:
    vk::Image handle;
  };
#endif
}
#endif

