#ifndef GCT_SWAPCHAIN_CREATE_INFO_HPP
#define GCT_SWAPCHAIN_CREATE_INFO_HPP
#include <cstdint>
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class surface_t;
  class surface_format_t;
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  class swapchain_create_info_t : public chained_t {
  public:
    using self_type = swapchain_create_info_t;
    swapchain_create_info_t &set_surface(
      const std::shared_ptr< surface_t > &surface,
      const surface_format_t &format,
      std::uint32_t image_count
    );
    swapchain_create_info_t &set_surface(
      const std::shared_ptr< surface_t > &surface_
    );
private:
    std::shared_ptr< surface_t > surface;
public:
    const std::shared_ptr< surface_t > &get_surface() const { return surface; }

    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::SwapchainCreateInfoKHR )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupSwapchainCreateInfoKHR , device_group )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::ImageFormatListCreateInfo, format_list )
#elif defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ImageFormatListCreateInfoKHR, format_list )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SurfaceFullScreenExclusiveInfoEXT, full_screen_exclusive )
#ifdef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SurfaceFullScreenExclusiveWin32InfoEXT, full_screen_exclusive_win32 )
#endif
#endif
#ifdef VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SwapchainCounterCreateInfoEXT, counter )
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::SwapchainDisplayNativeHdrCreateInfoAMD, display_antive_hdr )
#endif
  };
  void to_json( nlohmann::json &root, const swapchain_create_info_t &v );
  void from_json( const nlohmann::json &root, swapchain_create_info_t &v );
#endif
}

#endif

