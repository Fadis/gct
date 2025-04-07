#include <nlohmann/json.hpp>
#include <gct/image_shrink_info.hpp>
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
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
#include <vulkan2json/ExternalFormatQNX.hpp>
#endif 
#ifdef VK_VERSION_1_1
#include <vulkan2json/ExternalMemoryImageCreateInfo.hpp>
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
#include <vulkan2json/ExternalMemoryImageCreateInfoKHR.hpp>
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
#include <vulkan2json/ExternalMemoryImageCreateInfoNV.hpp>
#endif
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
#include <vulkan2json/ImageAlignmentControlCreateInfoMESA.hpp>
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
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( external_format_qnx ) 
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( alignment_control )
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
    LIBGCT_ARRAY_OF_TO_JSON( video_profiles, pProfiles, video_profile_info )
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
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( external_format_qnx ) 
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( alignment_control )
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
    LIBGCT_ARRAY_OF_FROM_JSON( video_profiles, pProfiles, video_profile_info )
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
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_format_qnx ) 
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
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( alignment_control )
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( image_compression_control, CompressionControlPlaneCount, PFixedRateFlags, fixed_rate_flag )
    LIBGCT_EXTENSION_REBUILD_CHAIN( image_compression_control )
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( drm_format_modifier_explicit, DrmFormatModifierPlaneCount, PPlaneLayouts, plane_layout )
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier_explicit ) 
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( drm_format_modifier_list, DrmFormatModifierCount, PDrmFormatModifiers, drm_format_modifier )
    LIBGCT_EXTENSION_REBUILD_CHAIN( drm_format_modifier_list ) 
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( format_list, ViewFormatCount, PViewFormats, format )
    LIBGCT_EXTENSION_REBUILD_CHAIN( format_list ) 
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
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( video_profiles, ProfileCount, PProfiles, video_profile_info )
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profiles )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  image_create_info_t &image_create_info_t::shrink(
    const image_shrink_info &ci
  ) {
    basic.extent.width /= ci.extent.x;
    basic.extent.height /= ci.extent.y;
    basic.extent.depth /= ci.extent.z;
    return *this;
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
  bool operator==( const image_create_info_t &l, const image_create_info_t &r ) {
    l.rebuild_chain();
    r.rebuild_chain();
    if( l.get_basic().flags != r.get_basic().flags ) return false;
    if( l.get_basic().imageType != r.get_basic().imageType ) return false;
    if( l.get_basic().format != r.get_basic().format ) return false;
    if( l.get_basic().extent != r.get_basic().extent ) return false;
    if( l.get_basic().arrayLayers != r.get_basic().arrayLayers ) return false;
    if( l.get_basic().mipLevels != r.get_basic().mipLevels ) return false;
    if( l.get_basic().samples != r.get_basic().samples ) return false;
    if( l.get_basic().tiling != r.get_basic().tiling ) return false;
    if( l.get_basic().usage != r.get_basic().usage ) return false;
    if( l.get_basic().sharingMode != r.get_basic().sharingMode ) return false;
    if( l.get_basic().queueFamilyIndexCount != r.get_basic().queueFamilyIndexCount ) return false;
    if( l.get_basic().queueFamilyIndexCount ) {
      for( std::uint32_t i = 0u; i != l.get_basic().queueFamilyIndexCount; ++i ) {
        if( l.get_basic().pQueueFamilyIndices[ i ] != r.get_basic().pQueueFamilyIndices[ i ] ) return false;
      }
    }
    if( l.get_basic().initialLayout != r.get_basic().initialLayout ) return false;

#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    if( l.has_buffer_collection_image() != r.has_buffer_collection_image() ) return false;
    if( l.has_buffer_collection_image() ) {
      if( l.get_buffer_collection_image().collection != r.get_buffer_collection_image().collection ) return false;
      if( l.get_buffer_collection_image().index != r.get_buffer_collection_image().index ) return false;
    }
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    if( l.has_dedicated_allocation_image() != r.has_dedicated_allocation_image() ) return false;
    if( l.has_dedicated_allocation_image() ) {
      if( l.get_dedicated_allocation_image().dedicatedAllocation != r.get_dedicated_allocation_image().dedicatedAllocation ) return false;
    }
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    if( l.has_export_metal_object() != r.has_export_metal_object() ) return false;
    if( l.has_export_metal_object() ) {
      if( l.get_export_metal_object().exportObjectType != r.get_export_metal_object().exportObjectType ) return false;
    }
#endif
#ifdef VK_QNX_EXTERNAL_MEMORY_SCREEN_BUFFER_EXTENSION_NAME
    if( l.has_external_format_qnx() != r.has_external_format_qnx() ) return false;
    if( l.has_external_format_qnx() ) {
      if( l.get_external_format_qnx().externalFormat != r.get_external_format_qnx().externalFormat ) return false;
    }
#endif
#ifdef VK_ANDROID_EXTERNAL_MEMORY_ANDROID_HARDWARE_BUFFER_EXTENSION_NAME
    if( l.has_external_format() != r.has_external_format() ) return false;
    if( l.has_external_format() ) {
      if( l.get_external_format().externalFormat != r.get_external_format().externalFormat ) return false;
    }
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    if( l.has_external_memory_image() != r.has_external_memory_image() ) return false;
    if( l.has_external_memory_image() ) {
      if( l.get_external_memory_image().handleTypes != r.get_external_memory_image().handleTypes ) return false;
    }
#endif
#ifdef VK_NV_EXTERNAL_MEMORY_EXTENSION_NAME
    if( l.has_external_memory_image_nv() != r.has_external_memory_image_nv() ) return false;
    if( l.has_external_memory_image_nv() ) {
      if( l.get_external_memory_image_nv().handleTypes != r.get_external_memory_image_nv().handleTypes ) return false;
    }
#endif
#ifdef VK_MESA_IMAGE_ALIGNMENT_CONTROL_EXTENSION_NAME
    if( l.has_alignment_control() != r.has_alignment_control() ) return false;
    if( l.has_alignment_control() ) {
      if( l.get_alignment_control().maximumRequestedAlignment != r.get_alignment_control().maximumRequestedAlignment ) return false;
    }
#endif
#ifdef VK_EXT_IMAGE_COMPRESSION_CONTROL_EXTENSION_NAME
    if( l.has_image_compression_control() != r.has_image_compression_control() ) return false;
    if( l.has_image_compression_control() ) {
      if( l.get_image_compression_control().flags != r.get_image_compression_control().flags ) return false;
      if( l.get_image_compression_control().compressionControlPlaneCount ) {
        for( std::uint32_t i = 0u; i != l.get_image_compression_control().compressionControlPlaneCount; ++i ) {
          if( l.get_image_compression_control().pFixedRateFlags[ i ] != r.get_image_compression_control().pFixedRateFlags[ i ] ) return false;
        }
      }
    }
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    if( l.has_drm_format_modifier_explicit() != r.has_drm_format_modifier_explicit() ) return false;
    if( l.has_drm_format_modifier_explicit() ) {
      if( l.get_drm_format_modifier_explicit().drmFormatModifier != r.get_drm_format_modifier_explicit().drmFormatModifier ) return false;
      if( l.get_drm_format_modifier_explicit().drmFormatModifierPlaneCount != r.get_drm_format_modifier_explicit().drmFormatModifierPlaneCount ) return false;
      if( l.get_drm_format_modifier_explicit().drmFormatModifierPlaneCount ) {
        for( std::uint32_t i = 0u; i != l.get_drm_format_modifier_explicit().drmFormatModifierPlaneCount; ++i ) {
          if( l.get_drm_format_modifier_explicit().pPlaneLayouts[ i ] != r.get_drm_format_modifier_explicit().pPlaneLayouts[ i ] ) return false;
        }
      }
    }
#endif
#ifdef VK_EXT_IMAGE_DRM_FORMAT_MODIFIER_EXTENSION_NAME
    if( l.has_drm_format_modifier_list() != r.has_drm_format_modifier_list() ) return false;
    if( l.has_drm_format_modifier_list() ) {
      if( l.get_drm_format_modifier_list().drmFormatModifierCount != r.get_drm_format_modifier_list().drmFormatModifierCount ) return false;
      if( l.get_drm_format_modifier_list().drmFormatModifierCount ) {
        for( std::uint32_t i = 0u; i != l.get_drm_format_modifier_list().drmFormatModifierCount; ++i ) {
          if( l.get_drm_format_modifier_list().pDrmFormatModifiers[ i ] != r.get_drm_format_modifier_list().pDrmFormatModifiers[ i ] ) return false;
        }
      }
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME)
    if( l.has_format_list() != r.has_format_list() ) return false;
    if( l.has_format_list() ) {
      if( l.get_format_list().viewFormatCount != r.get_format_list().viewFormatCount ) return false;
      if( l.get_format_list().viewFormatCount ) {
        for( std::uint32_t i = 0u; i != l.get_format_list().viewFormatCount; ++i ) {
          if( l.get_format_list().pViewFormats[ i ] != r.get_format_list().pViewFormats[ i ] ) return false;
        }
      }
    }
#endif
#if defined(VK_VERSION_1_2) || defined(VK_EXT_SEPARATE_STENCIL_USAGE_EXTENSION_NAME)
    if( l.has_stencil_usage() != r.has_stencil_usage() ) return false;
    if( l.has_stencil_usage() ) {
      if( l.get_stencil_usage().stencilUsage != r.get_stencil_usage().stencilUsage ) return false;
    }
#endif
#if ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_VERSION_1_1) ) || ( defined(VK_KHR_SWAPCHAIN_EXTENSION_NAME) && defined(VK_KHR_DEVICE_GROUP_EXTENSION_NAME) )
    if( l.has_swapchain() != r.has_swapchain() ) return false;
    if( l.has_swapchain() ) {
      if( l.get_swapchain().swapchain != r.get_swapchain().swapchain ) return false;
    }
#endif
#ifdef VK_EXT_METAL_OBJECTS_EXTENSION_NAME
    if( l.has_import_metal_os_surface() != r.has_import_metal_os_surface() ) return false;
    if( l.has_import_metal_os_surface() ) {
      if( l.get_import_metal_os_surface().ioSurface != r.get_import_metal_os_surface().ioSurface ) return false;
    }
    if( l.has_import_metal_texture() != r.has_import_metal_texture() ) return false;
    if( l.has_import_metal_texture() ) {
      if( l.get_import_metal_texture().plane != r.get_import_metal_texture().plane ) return false;
      if( l.get_import_metal_texture().mtlTexture != r.get_import_metal_texture().mtlTexture ) return false;
    }
#endif
#ifdef VK_EXT_DESCRIPTOR_BUFFER_EXTENSION_NAME
    if( l.has_opaque_capture_descriptor_data() != r.has_opaque_capture_descriptor_data() ) return false;
    /*if( l.has_opaque_capture_descriptor_data() ) {
      if( l.get_opaque_capture_descriptor_data().opaqueCaptureDescriptorData != r.get_opaque_capture_descriptor_data().opaqueCaptureDescriptorData ) return false;
    }*/
#endif
#ifdef VK_NV_OPTICAL_FLOW_EXTENSION_NAME
    if( l.has_optical_flow_image_format() != r.has_optical_flow_image_format() ) return false;
    if( l.has_optical_flow_image_format() ) {
      if( l.get_optical_flow_image_format().usage != r.get_optical_flow_image_format().usage ) return false;
    }
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    if( l.has_video_profile() != r.has_video_profile() ) return false;
    if( l.has_video_profile() ) {
      if( l.get_video_profile().videoCodecOperation != r.get_video_profile().videoCodecOperation ) return false;
      if( l.get_video_profile().chromaSubsampling != r.get_video_profile().chromaSubsampling ) return false;
      if( l.get_video_profile().lumaBitDepth != r.get_video_profile().lumaBitDepth ) return false;
      if( l.get_video_profile().chromaBitDepth != r.get_video_profile().chromaBitDepth ) return false;
    }
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    if( l.has_video_profiles() != r.has_video_profiles() ) return false;
    if( l.has_video_profiles() ) {
      if( l.get_video_profiles().profileCount != r.get_video_profiles().profileCount ) return false;
      if( l.get_video_profiles().profileCount ) {
        for( std::uint32_t i = 0u; i != l.get_video_profiles().profileCount; ++i ) {
          if( l.get_video_profiles().pProfiles[ i ].videoCodecOperation != r.get_video_profiles().pProfiles[ i ].videoCodecOperation ) return false;
          if( l.get_video_profiles().pProfiles[ i ].chromaSubsampling != r.get_video_profiles().pProfiles[ i ].chromaSubsampling ) return false;
          if( l.get_video_profiles().pProfiles[ i ].lumaBitDepth != r.get_video_profiles().pProfiles[ i ].lumaBitDepth ) return false;
          if( l.get_video_profiles().pProfiles[ i ].chromaBitDepth != r.get_video_profiles().pProfiles[ i ].chromaBitDepth ) return false;
        }
      }
    }
#endif
    return true;
  }
  bool operator!=( const image_create_info_t &l, const image_create_info_t &r ) {
    return !operator==( l, r );
  }
}

