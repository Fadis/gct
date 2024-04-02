#include <nlohmann/json.hpp>
#include <gct/surface.hpp>
#include <gct/swapchain_create_info.hpp>
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
#include <vulkan2json/SwapchainCreateInfoKHR.hpp>
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
#include <vulkan2json/DeviceGroupSwapchainCreateInfoKHR.hpp>
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
#include <vulkan2json/ImageCompressionControlEXT.hpp>
#include <vulkan2json/ImageCompressionFixedRateFlagsEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/ImageFormatListCreateInfo.hpp>
#include <vulkan2json/Format.hpp>
#elif defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
#include <vulkan2json/ImageFormatListCreateInfoKHR.hpp>
#include <vulkan2json/Format.hpp>
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
#include <vulkan2json/SurfaceFullScreenExclusiveInfoEXT.hpp>
#ifdef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
#include <vulkan2json/SurfaceFullScreenExclusiveWin32InfoEXT.hpp>
#endif
#endif
#ifdef VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
#include <vulkan2json/SwapchainCounterCreateInfoEXT.hpp>
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
#include <vulkan2json/SwapchainDisplayNativeHdrCreateInfoAMD.hpp>
#endif
#ifdef VK_NV_LOW_LATENCY_2_EXTENSION_NAME
#include <vulkan2json/SwapchainLatencyCreateInfoNV.hpp>
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
#include <vulkan2json/SwapchainPresentBarrierCreateInfoNV.hpp>
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
#include <vulkan2json/SwapchainPresentModesCreateInfoEXT.hpp>
#include <vulkan2json/SwapchainPresentScalingCreateInfoEXT.hpp>
#include <vulkan2json/PresentModeKHR.hpp>
#endif
#endif
#include <gct/array_of.hpp>

namespace gct {
#ifdef VK_KHR_SWAPCHAIN_EXTENSION_NAME
  void to_json( nlohmann::json &root, const swapchain_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pQueueFamilyIndices, queue_family_index )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( device_group )
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( compression_control )
    LIBGCT_ARRAY_OF_TO_JSON( compression_control, pFixedRateFlags, fixed_rate_flag )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( format_list )
    LIBGCT_ARRAY_OF_TO_JSON( format_list, pViewFormats, format )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( full_screen_exclusive )
#ifdef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( full_screen_exclusive_win32 )
#endif
#endif
#ifdef VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( counter )
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( display_antive_hdr )
#endif
#ifdef VK_NV_LOW_LATENCY_2_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( latency )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( present_barrier )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( present_modes )
    LIBGCT_ARRAY_OF_TO_JSON( present_modes, pPresentModes, present_mode )
    LIBGCT_EXTENSION_TO_JSON( present_scaling )
#endif
  }
  void from_json( const nlohmann::json &root, swapchain_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to swapchain_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pQueueFamilyIndices, queue_family_index )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( device_group )
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( compression_control )
    LIBGCT_ARRAY_OF_FROM_JSON( compression_control, pFixedRateFlags, fixed_rate_flag )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( format_list )
    LIBGCT_ARRAY_OF_FROM_JSON( format_list, pViewFormats, format )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( full_screen_exclusive )
#ifdef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( full_screen_exclusive_win32 )
#endif
#endif
#ifdef VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( counter )
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( display_antive_hdr )
#endif
#ifdef VK_NV_LOW_LATENCY_2_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( latency )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( present_barrier )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( present_modes )
    LIBGCT_ARRAY_OF_FROM_JSON( present_modes, pPresentModes, present_mode )
    LIBGCT_EXTENSION_FROM_JSON( present_scaling )
#endif
  }
  swapchain_create_info_t &swapchain_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, QueueFamilyIndexCount, PQueueFamilyIndices, queue_family_index )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_group )
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( compression_control, CompressionControlPlaneCount, PFixedRateFlags, fixed_rate_flag )
    LIBGCT_EXTENSION_REBUILD_CHAIN( compression_control )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( format_list, ViewFormatCount, PViewFormats, format )
    LIBGCT_EXTENSION_REBUILD_CHAIN( format_list )
#endif
#ifdef VK_EXT_FULL_SCREEN_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( full_screen_exclusive )
#ifdef VK_KHR_WIN32_SURFACE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( full_screen_exclusive_win32 )
#endif
#endif
#ifdef VK_EXT_DISPLAY_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( counter )
#endif
#ifdef VK_AMD_DISPLAY_NATIVE_HDR_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( display_antive_hdr )
#endif
#ifdef VK_NV_LOW_LATENCY_2_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( latency )
#endif
#ifdef VK_NV_PRESENT_BARRIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( present_barrier )
#endif
#ifdef VK_EXT_SWAPCHAIN_MAINTENANCE_1_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( present_modes, PresentModeCount, PPresentModes, present_mode )
    LIBGCT_EXTENSION_REBUILD_CHAIN( present_modes )
    LIBGCT_EXTENSION_REBUILD_CHAIN( present_scaling )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  swapchain_create_info_t &swapchain_create_info_t::set_surface(
    const std::shared_ptr< surface_t > &surface_,
    const surface_format_t &format,
    std::uint32_t image_count
  ) {
    surface = surface_;
    auto modified = get_basic();
    modified
      .setSurface( **surface )
      .setMinImageCount(
        std::max(
          std::min(
            image_count,
            surface->get_caps().get_basic().maxImageCount
          ),
          surface->get_caps().get_basic().minImageCount
        )
      )
      .setImageFormat( format.basic.format )
      .setImageColorSpace( format.basic.colorSpace )
      .setImageExtent( surface->get_caps().get_basic().currentExtent )
      .setImageArrayLayers( 1 )
      .setImageUsage(
        vk::ImageUsageFlagBits::eColorAttachment|
        vk::ImageUsageFlagBits::eTransferDst|
        vk::ImageUsageFlagBits::eStorage
      )
      .setPreTransform(
        ( surface->get_caps().get_basic().supportedTransforms & vk::SurfaceTransformFlagBitsKHR::eIdentity ) ?
        vk::SurfaceTransformFlagBitsKHR::eIdentity :
        surface->get_caps().get_basic().currentTransform
      );
    set_basic( std::move( modified ) );
    return *this;
  }
  swapchain_create_info_t &swapchain_create_info_t::set_surface(
    const std::shared_ptr< surface_t > &surface_
  ) {
    const auto &formats = surface_->get_caps().get_formats();
    if( formats.empty() ) {
      throw -1;
    }
    static const std::vector< vk::Format > supported_formats {
      vk::Format::eA2B10G10R10UnormPack32,
      vk::Format::eA2R10G10B10UnormPack32,
      vk::Format::eB8G8R8A8Unorm,
      vk::Format::eR8G8B8A8Unorm,
      vk::Format::eA8B8G8R8UnormPack32,
      vk::Format::eB5G5R5A1UnormPack16,
      vk::Format::eA1R5G5B5UnormPack16
    };
    for( const auto &s: supported_formats ) {
      auto selected_format = std::find_if( formats.begin(), formats.end(), [&]( const auto &v ) { return v.basic.format == s && v.basic.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear; } );
      if( selected_format != formats.end() ) {
        return set_surface( surface_, *selected_format, surface_->get_caps().get_basic().minImageCount );
      }
    }
    throw -1;
  }
#endif
}

