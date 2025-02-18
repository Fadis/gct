#ifndef GCT_IMAGE_VIEW_CREATE_INFO_HPP
#define GCT_IMAGE_VIEW_CREATE_INFO_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class image_view_create_info_t : public chained_t {
  public:
    using self_type = image_view_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::ImageViewCreateInfo )
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::ImageViewASTCDecodeModeEXT , astc_decode_mode )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::ImageViewUsageCreateInfo, usage )
#elif define(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ImageViewUsageCreateInfoKHR, usage )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::SamplerYcbcrConversionInfo , external_format )
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::SamplerYcbcrConversionInfoKHR , external_format )
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH264ProfileInfoKHR , video_decode_h264_profile )
#elif defined(VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME)
#if VK_EXT_VIDEO_DECODE_H264_SPEC_VERSION < 6
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH264ProfileEXT , video_decode_h264_profile )
#else
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH264ProfileInfoEXT , video_decode_h264_profile )
#endif
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH265ProfileInfoKHR , video_decode_h265_profile )
#elif defined(VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME)
#if VK_EXT_VIDEO_DECODE_H265_SPEC_VERSION < 4
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH265ProfileEXT , video_decode_h265_profile )
#else
    LIBGCT_EXTENSION_SETTER( vk::VideoDecodeH265ProfileInfoEXT , video_decode_h265_profile )
#endif
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H264_EXTENSION_NAME
#if VK_EXT_VIDEO_ENCODE_H264_SPEC_VERSION < 8
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeH264ProfileEXT , video_encode_h264_profile )
#else
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeH264ProfileInfoEXT , video_encode_h264_profile )
#endif
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H265_EXTENSION_NAME
#if VK_EXT_VIDEO_ENCODE_H265_SPEC_VERSION < 8
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeH265ProfileEXT , video_encode_h265_profile )
#else
    LIBGCT_EXTENSION_SETTER( vk::VideoEncodeH265ProfileInfoEXT , video_encode_h265_profile )
#endif
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
  };
  void to_json( nlohmann::json &root, const image_view_create_info_t &v );
  void from_json( const nlohmann::json &root, image_view_create_info_t &v );
}

#endif

