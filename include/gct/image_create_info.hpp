#ifndef GCT_IMAGE_CREATE_INFO_HPP
#define GCT_IMAGE_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
#include <gct/setter.hpp>
#include <gct/color_space.hpp>

namespace gct {
  class image_create_info_t : public chained_t {
  public:
    using self_type = image_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ImageCreateInfo )
    LIBGCT_ARRAY_OF_SMALL( std::uint32_t, queue_family_index )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::BufferCollectionImageCreateInfoFUCHSIA , buffer_collection_image )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DedicatedAllocationImageCreateInfoNV , dedicated_allocation_image )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExportMetalObjectCreateInfoEXT, export_metal_object )
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExternalFormatANDROID , external_format )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::ExternalMemoryImageCreateInfo , external_memory_image )
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ExternalMemoryImageCreateInfoKHR , external_memory_image )
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ExternalMemoryImageCreateInfoNV , external_memory_image_nv )
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImageCompressionControlEXT , image_compression_control )
    LIBGCT_ARRAY_OF_SMALL( vk::ImageCompressionFixedRateFlagsEXT, fixed_rate_flag )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImageDrmFormatModifierExplicitCreateInfoEXT , drm_format_modifier_explicit )
    LIBGCT_ARRAY_OF( vk::SubresourceLayout, plane_layout )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImageDrmFormatModifierListCreateInfoEXT , drm_format_modifier_list )
    LIBGCT_ARRAY_OF_SMALL( std::uint64_t, drm_format_modifier )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::ImageFormatListCreateInfo , format_list )
#elif defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ImageFormatListCreateInfoKHR , format_list )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_SMALL( vk::Format, format )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::ImageStencilUsageCreateInfo , stencil_usage )
#elif defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ImageStencilUsageCreateInfoEXT , stencil_usage )
#endif
#if defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1)
    LIBGCT_EXTENSION_SETTER( vk::ImageSwapchainCreateInfoKHR , swapchain )
#elif defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ImageSwapchainCreateInfoKHR , swapchain )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImportMetalIOSurfaceInfoEXT , import_metal_os_surface )
    LIBGCT_EXTENSION_SETTER( vk::ImportMetalTextureInfoEXT , import_metal_texture )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::OpaqueCaptureDescriptorDataCreateInfoEXT , opaque_capture_descriptor_data )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::OpticalFlowImageFormatInfoNV , optical_flow_image_format )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
#if VK_KHR_VIDEO_QUEUE_SPEC_VERSION < 5
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileKHR , video_profile )
    LIBGCT_EXTENSION_SETTER( vk::VideoProfilesKHR , video_profiles )
    LIBGCT_ARRAY_OF( vk::VideoProfileKHR, video_profile_info )
#else
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileInfoKHR , video_profile ) 
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileListInfoKHR , video_profiles )
    LIBGCT_ARRAY_OF( vk::VideoProfileInfoKHR, video_profile_info )
#endif
#endif
  public:
    LIBGCT_SETTER( profile );
    const color_profile &get_profile() const {
      return profile;
    }
  private:
    color_profile profile;
  };
  vk::ImageCreateInfo basic_2d_image( std::uint32_t width, std::uint32_t height );
  vk::ImageCreateInfo basic_3d_image( std::uint32_t width, std::uint32_t height, std::uint32_t depth );
  void to_json( nlohmann::json &root, const image_create_info_t &v );
  void from_json( const nlohmann::json &root, image_create_info_t &v );
}

#endif

