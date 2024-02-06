#include <nlohmann/json.hpp>
#include <gct/image_create_info.hpp>
#include <vulkan2json/ImageCreateInfo.hpp>
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
#include <vulkan2json/BufferCollectionImageCreateInfoFUCHSIA.hpp>
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
#include <vulkan2json/DedicatedAllocationImageCreateInfoNV.hpp>
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
#include <vulkan2json/ExportMetalObjectCreateInfoEXT.hpp>
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
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
#include <vulkan2json/ImageCompressionControlEXT.hpp>
#include <vulkan2json/ImageCompressionFixedRateFlagsEXT.hpp>
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
#include <vulkan2json/ImageDrmFormatModifierExplicitCreateInfoEXT.hpp>
#include <vulkan2json/SubresourceLayout.hpp>
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
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
#include <vulkan2json/ImportMetalIOSurfaceInfoEXT.hpp>
#include <vulkan2json/ImportMetalTextureInfoEXT.hpp>
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
#include <vulkan2json/OpaqueCaptureDescriptorDataCreateInfoEXT.hpp>
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
#include <vulkan2json/OpticalFlowImageFormatInfoNV.hpp>
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
    LIBGCT_ARRAY_OF_TO_JSON( basic, pQueueFamilyIndices, queue_family_index )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( buffer_collection_image ) 
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( dedicated_allocation_image ) 
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( export_metal_object )
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
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( image_compression_control )
    LIBGCT_ARRAY_OF_TO_JSON( image_compression_control, pFixedRateFlags, fixed_rate_flag )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm_format_modifier_explicit ) 
    LIBGCT_ARRAY_OF_TO_JSON( drm_format_modifier_explicit, pPlaneLayouts, plane_layout )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( drm_format_modifier_list ) 
    LIBGCT_ARRAY_OF_TO_JSON( drm_format_modifier_list, pDrmFormatModifiers, drm_format_modifier )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( format_list ) 
    LIBGCT_ARRAY_OF_TO_JSON( format_list, pViewFormats, format )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( stencil_usage )
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    LIBGCT_EXTENSION_TO_JSON( swapchain )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( import_metal_os_surface )
    LIBGCT_EXTENSION_TO_JSON( import_metal_texture )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( opaque_capture_descriptor_data )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( optical_flow_image_format )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profiles )
    LIBGCT_ARRAY_OF_TO_JSON( video_profiles, pProfiles, profile )
#endif
  }
  void from_json( const nlohmann::json &root, image_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to image_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pQueueFamilyIndices, queue_family_index )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( buffer_collection_image ) 
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( dedicated_allocation_image ) 
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( export_metal_object )
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
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( image_compression_control )
    LIBGCT_ARRAY_OF_FROM_JSON( image_compression_control, pFixedRateFlags, fixed_rate_flag )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( drm_format_modifier_explicit ) 
    LIBGCT_ARRAY_OF_FROM_JSON( drm_format_modifier_explicit, pPlaneLayouts, plane_layout )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( drm_format_modifier_list ) 
    LIBGCT_ARRAY_OF_FROM_JSON( drm_format_modifier_list, pDrmFormatModifiers, drm_format_modifier )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( format_list ) 
    LIBGCT_ARRAY_OF_FROM_JSON( format_list, pViewFormats, format )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( stencil_usage )
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    LIBGCT_EXTENSION_FROM_JSON( swapchain )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( import_metal_os_surface )
    LIBGCT_EXTENSION_FROM_JSON( import_metal_texture )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( opaque_capture_descriptor_data )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( optical_flow_image_format )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profiles )
    LIBGCT_ARRAY_OF_FROM_JSON( video_profiles, pProfiles, profile )
#endif
  }
  image_create_info_t &image_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( basic.mipLevels == 0u ) basic.setMipLevels( 1u );
    if( basic.arrayLayers == 0u ) basic.setArrayLayers( 1u );
    if( basic.extent.depth == 0u ) basic.extent.setDepth( 1u );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, QueueFamilyIndexCount, PQueueFamilyIndices, queue_family_index )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_collection_image ) 
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( dedicated_allocation_image ) 
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( export_metal_object )
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
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_compression_control )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( image_compression_control, CompressionControlPlaneCount, PFixedRateFlags, fixed_rate_flag )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier_explicit ) 
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( drm_format_modifier_explicit, DrmFormatModifierPlaneCount, PPlaneLayouts, plane_layout )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier_list ) 
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( drm_format_modifier_list, DrmFormatModifierCount, PDrmFormatModifiers, drm_format_modifier )
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( format_list ) 
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( format_list, ViewFormatCount, PViewFormats, format )
    if( !format.empty() ) {
      basic.flags |= vk::ImageCreateFlagBits::eMutableFormat;
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( stencil_usage )
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    LIBGCT_EXTENSION_REBUILD_CHAIN( swapchain )
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( import_metal_os_surface )
    LIBGCT_EXTENSION_REBUILD_CHAIN( import_metal_texture )
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( opaque_capture_descriptor_data )
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( optical_flow_image_format )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profiles )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( video_profiles, ProfileCount, PProfiles, profile )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  vk::ImageCreateInfo basic_2d_image( std::uint32_t width, std::uint32_t height ) {
    return vk::ImageCreateInfo()
      .setImageType( vk::ImageType::e2D )
      .setFormat( vk::Format::eR32G32B32A32Sfloat )
      .setExtent( { width, height, 1 } )
      .setMipLevels( 1 )
      .setArrayLayers( 1 )
      .setSamples( vk::SampleCountFlagBits::e1 )
      .setTiling( vk::ImageTiling::eOptimal )
      .setInitialLayout( vk::ImageLayout::eUndefined );
  }
  vk::ImageCreateInfo basic_3d_image( std::uint32_t width, std::uint32_t height, std::uint32_t depth ) {
    return vk::ImageCreateInfo()
      .setImageType( vk::ImageType::e3D )
      .setFormat( vk::Format::eR32G32B32A32Sfloat )
      .setExtent( { width, height, depth } )
      .setMipLevels( 1 )
      .setArrayLayers( 1 )
      .setSamples( vk::SampleCountFlagBits::e1 )
      .setTiling( vk::ImageTiling::eOptimal )
      .setInitialLayout( vk::ImageLayout::eUndefined );
  }
}

