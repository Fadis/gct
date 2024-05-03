#include <nlohmann/json.hpp>
#include <gct/query_pool_create_info.hpp>
#include <vulkan2json/QueryPoolCreateInfo.hpp>
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
#include <vulkan2json/QueryPoolPerformanceCreateInfoKHR.hpp>
#endif
#ifdef VK_INTEL_PERFORMANCE_QUERY_EXTENSION_NAME
#include <vulkan2json/QueryPoolPerformanceQueryCreateInfoINTEL.hpp>
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
#include <vulkan2json/QueryPoolVideoEncodeFeedbackCreateInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME
#include <vulkan2json/VideoDecodeAV1ProfileInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
#include <vulkan2json/VideoDecodeH264ProfileInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
#include <vulkan2json/VideoDecodeH265ProfileInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME
#include <vulkan2json/VideoDecodeUsageInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME
#include <vulkan2json/VideoEncodeH264ProfileInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME
#include <vulkan2json/VideoEncodeH265ProfileInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
#include <vulkan2json/VideoEncodeUsageInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
#include <vulkan2json/VideoProfileInfoKHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const query_pool_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( performance )
#endif
#ifdef VK_INTEL_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( performance_query )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_encode_feedback )
#endif
#ifdef VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_av1_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_usage )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_encode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_encoder_usage )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profile )
#endif
  }
  void from_json( const nlohmann::json &root, query_pool_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to query_pool_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( performance )
#endif
#ifdef VK_INTEL_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( performance_query )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_encode_feedback )
#endif
#ifdef VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_av1_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_usage )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_encode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_encoder_usage )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profile )
#endif
  }
  query_pool_create_info_t &query_pool_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( performance )
#endif
#ifdef VK_INTEL_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( performance_query )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_encode_feedback )
#endif
#ifdef VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_av1_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_decode_usage )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_encode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_encoder_usage )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video_profile )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

