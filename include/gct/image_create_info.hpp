#ifndef GCT_IMAGE_CREATE_INFO_HPP
#define GCT_IMAGE_CREATE_INFO_HPP
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class image_create_info_t : public chained_t {
  public:
    using self_type = image_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ImageCreateInfo )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::BufferCollectionImageCreateInfoFUCHSIA , buffer_collection_image )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DedicatedAllocationImageCreateInfoNV , dedicated_allocation_image )
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
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImageDrmFormatModifierExplicitCreateInfoEXT , drm_format_modifier_explicit )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImageDrmFormatModifierListCreateInfoEXT , drm_format_modifier_list )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::ImageFormatListCreateInfo , format_list )
#elif defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ImageFormatListCreateInfoKHR , format_list )
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
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
#if VK_KHR_VIDEO_QUEUE_SPEC_VERSION < 5
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileKHR , video_profile )
    LIBGCT_EXTENSION_SETTER( vk::VideoProfilesKHR , video_profiles )
#else
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileInfoKHR , video_profile ) 
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileListInfoKHR , video_profiles )
#endif
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
  private:
    std::vector< vk::Format > format_list_formats;
  public:
    image_create_info_t &add_format( vk::Format );
    image_create_info_t &clear_format();
    const std::vector< vk::Format > &get_format_list_formats() const { return format_list_formats; }
#endif
  };
  void to_json( nlohmann::json &root, const image_create_info_t &v );
  void from_json( const nlohmann::json &root, image_create_info_t &v );
}

#endif

