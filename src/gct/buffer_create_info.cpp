#include <gct/buffer_create_info.hpp>

namespace gct {
  buffer_create_info_t &buffer_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( buffer_collection ) 
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( device_address )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_REBUILD_CHAIN( opaque_capture_address )
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( opaque_capture_address )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( dedicated_allocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory )
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profile )
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profiles )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

