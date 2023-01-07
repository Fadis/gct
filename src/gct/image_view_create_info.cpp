#include <gct/image_view_create_info.hpp>
#include <vulkan2json/ImageViewCreateInfo.hpp>
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
#include <vulkan2json/ImageViewASTCDecodeModeEXT.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/ImageViewUsageCreateInfo.hpp>
#elif define(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
#include <vulkan2json/ImageViewUsageCreateInfoKHR.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/SamplerYcbcrConversionInfo.hpp>
#elif defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
#include <vulkan2json/SamplerYcbcrConversionInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
#include <vulkan2json/VkVideoDecodeH264ProfileInfoKHR.hpp>
#elif defined(VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME)
#if VK_EXT_VIDEO_DECODE_H264_SPEC_VERSION < 6
#include <vulkan2json/VideoDecodeH264ProfileEXT.hpp>
#else
#include <vulkan2json/VideoDecodeH264ProfileInfoEXT.hpp>
#endif
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
#include <vulkan2json/VkVideoDecodeH265ProfileInfoKHR.hpp>
#elif defined(VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME)
#if VK_EXT_VIDEO_DECODE_H265_SPEC_VERSION < 4
#include <vulkan2json/VideoDecodeH265ProfileEXT.hpp>
#else
#include <vulkan2json/VideoDecodeH265ProfileInfoEXT.hpp>
#endif
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H264_EXTENSION_NAME
#if VK_EXT_VIDEO_ENCODE_H264_SPEC_VERSION < 8
#include <vulkan2json/VideoEncodeH264ProfileEXT.hpp>
#else
#include <vulkan2json/VideoEncodeH264ProfileInfoEXT.hpp>
#endif
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H265_EXTENSION_NAME
#if VK_EXT_VIDEO_ENCODE_H265_SPEC_VERSION < 8
#include <vulkan2json/VideoEncodeH265ProfileEXT.hpp>
#else
#include <vulkan2json/VideoEncodeH265ProfileInfoEXT.hpp>
#endif
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
  void to_json( nlohmann::json &root, const image_view_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( astc_decode_mode )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( usage )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( external_format )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_h265_profile )
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_encode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profile )
    LIBGCT_EXTENSION_TO_JSON( video_profiles )
#endif
  }
  void from_json( const nlohmann::json &root, image_view_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to image_view_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_ASTC_DECODE_MODE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( astc_decode_mode )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( usage )
#endif
#if defined(VK_VERSION_1_1) || defined(VK_KHR_SAMPLER_YCBCR_CONVERSION_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( external_format )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_h265_profile )
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_encode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_ENCODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_encode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profile )
    LIBGCT_EXTENSION_FROM_JSON( video_profiles )
#endif
  }
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

