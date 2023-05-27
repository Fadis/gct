#include <nlohmann/json.hpp>
#include <gct/image_create_info.hpp>
#include <vulkan2json/ImageCreateInfo.hpp>
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
#include <vulkan2json/BufferCollectionImageCreateInfoFUCHSIA.hpp>
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
#include <vulkan2json/DedicatedAllocationImageCreateInfoNV.hpp>
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
#include <vulkan2json/ExternalFormatANDROID.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/ExternalMemoryImageCreateInfo.hpp>
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
#include <vulkan2json/ExternalMemoryImageCreateInfoKHR.hpp>
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
#include <vulkan2json/ExternalMemoryImageCreateInfoNV.hpp>
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
#include <vulkan2json/ImageDrmFormatModifierExplicitCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
#include <vulkan2json/ImageDrmFormatModifierListCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/ImageFormatListCreateInfo.hpp>
#elif defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
#include <vulkan2json/ImageFormatListCreateInfoKHR.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/ImageStencilUsageCreateInfo.hpp>
#elif defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
#include <vulkan2json/ImageStencilUsageCreateInfoEXT.hpp>
#endif
#if defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1)
#include <vulkan2json/ImageSwapchainCreateInfoKHR.hpp>
#elif defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME)
#include <vulkan2json/ImageSwapchainCreateInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
#if VK_KHR_VIDEO_QUEUE_SPEC_VERSION < 5
#include <vulkan2json/VideoProfileKHR.hpp>
#include <vulkan2json/VideoProfilesKHR.hpp>
#else
#include <vulkan2json/VideoProfileInfoKHR.hpp>
#include <vulkan2json/VideoProfileListInfoKHR.hpp>
#endif
#endif

namespace gct {
  void to_json( nlohmann::json &root, const image_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( buffer_collection_image ) 
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( dedicated_allocation_image ) 
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_format ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( external_memory_image ) 
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_memory_image_nv ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm_format_modifier_explicit ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm_format_modifier_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( format_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( stencil_usage )
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    LIBGCT_EXTENSION_TO_JSON( swapchain )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profiles )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    if( !v.get_format_list_formats().empty() ) {
      if( root.find( "format_list" ) == root.end() )
        root[ "format_list" ] = nlohmann::json::object();
      root[ "format_list" ][ "pViewFormats" ] = v.get_format_list_formats();
    }
#endif
  }
  void from_json( const nlohmann::json &root, image_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to image_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( buffer_collection_image ) 
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( dedicated_allocation_image ) 
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( external_format ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( external_memory_image ) 
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( external_memory_image_nv ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( drm_format_modifier_explicit ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( drm_format_modifier_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( format_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( stencil_usage )
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    LIBGCT_EXTENSION_FROM_JSON( swapchain )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profiles )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    if( root.find( "format_list" ) != root.end() ) {
      if( root[ "format_list" ].find( "pViewFormats" ) != root.end() ) {
        if( !root[ "format_list" ][ "pViewFormats" ].is_array() ) throw incompatible_json( "The JSON is incompatible to image_create_info_t", __FILE__, __LINE__ );
        v.clear_format();
        for( auto &e: root[ "format_list" ][ "pViewFormats" ] )
          v.add_format( e );
      }
    }
#endif
  }
  image_create_info_t &image_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( basic.mipLevels == 0u ) basic.setMipLevels( 1u );
    if( basic.arrayLayers == 0u ) basic.setArrayLayers( 1u );
    if( basic.extent.depth == 0u ) basic.extent.setDepth( 1u );
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    if( !format_list_formats.empty() ) {
      set_format_list(
        vk::ImageFormatListCreateInfo()
          .setViewFormatCount( format_list_formats.size() )
          .setPViewFormats( format_list_formats.data() )
      );
    }
    if( has_format_list() ) {
      basic.flags |= vk::ImageCreateFlagBits::eMutableFormat;
    }
#endif
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_collection_image ) 
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( dedicated_allocation_image ) 
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_format ) 
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_image ) 
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory_image_nv ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier_explicit ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( format_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( stencil_usage )
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    LIBGCT_EXTENSION_REBUILD_CHAIN( swapchain )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profiles )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
  image_create_info_t &image_create_info_t::add_format( vk::Format v ) {
    format_list_formats.push_back( v );
    format_list.reset();
    chained = false;
    return *this;
  }
  image_create_info_t &image_create_info_t::clear_format() {
    format_list_formats.clear();
    format_list.reset();
    chained = false;
    return *this;
  }
#endif
}

