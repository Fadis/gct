#ifndef LIBGCT_INCLUDE_GCT_SWAPCHAIN_HPP
#define LIBGCT_INCLUDE_GCT_SWAPCHAIN_HPP

#include <memory>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/swapchain_create_info.hpp>

namespace gct {

#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  class device_t;
  class surface_t;
  class surface_format_t;
  class image_t;
  class semaphore_t;
  class swapchain_t : public created_from< device_t >, public std::enable_shared_from_this< swapchain_t > {
  public:
    swapchain_t(
      const std::shared_ptr< device_t >&,
      const swapchain_create_info_t&
    );
    vk::SwapchainKHR &operator*() {
      return *handle;
    }
    const vk::SwapchainKHR &operator*() const {
      return *handle;
    }
    vk::SwapchainKHR* operator->() {
      return &handle.get();
    }
    const vk::SwapchainKHR* operator->() const {
      return &handle.get();
    }
    const swapchain_create_info_t &get_props() const {
      return props;
    }
    std::vector< std::shared_ptr< image_t > > get_images();
    std::uint32_t acquire_next_image(
      const std::shared_ptr< semaphore_t > &
    );
  private:
    swapchain_create_info_t props;
    vk::UniqueHandle< vk::SwapchainKHR, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
#endif

}

#endif
