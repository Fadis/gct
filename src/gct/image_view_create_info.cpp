#include <gct/image_view_create_info.hpp>

namespace gct {
  image_view_create_info_t &image_view_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( astc_decode_mode )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( usage )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_format )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_h265_profile )
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_encode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profile )
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profiles )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

