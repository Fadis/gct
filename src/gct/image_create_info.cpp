#include <gct/image_create_info.hpp>

namespace gct {
  image_create_info_t &image_create_info_t::rebuild_chain() {
    if( basic.mipLevels == 0u ) basic.setMipLevels( 1u );
    if( basic.arrayLayers == 0u ) basic.setArrayLayers( 1u );
    if( basic.extent.depth == 0u ) basic.extent.setDepth( 1u );
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
}

