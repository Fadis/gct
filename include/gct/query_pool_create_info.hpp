#ifndef GCT_QUERY_POOL_CREATE_INFO_HPP
#define GCT_QUERY_POOL_CREATE_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>

namespace gct {
  class query_pool_create_info_t : public chained_t {
  public:
    using self_type = query_pool_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::QueryPoolCreateInfo )
#ifdef VK_KHR_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::QueryPoolPerformanceCreateInfoKHR , performance )
#endif
#ifdef VK_INTEL_PERFORMANCE_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::QueryPoolPerformanceQueryCreateInfoINTEL , performance_query )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::QueryPoolVideoEncodeFeedbackCreateInfoKHR, video_encode_feedback )
#endif
#ifdef VK_KHR_VIDEO_DECODE_AV1_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeAV1ProfileInfoKHR , video_decode_av1_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH264ProfileInfoKHR , video_decode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH265ProfileInfoKHR , video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeUsageInfoKHR , video_decode_usage )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeH264ProfileInfoKHR , video_encode_h264_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeH265ProfileInfoKHR , video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeUsageInfoKHR , video_encoder_usage )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoProfileInfoKHR , video_profile )
#endif
  };
  void to_json( nlohmann::json &root, const query_pool_create_info_t &v );
  void from_json( const nlohmann::json &root, query_pool_create_info_t &v );
}

#endif


