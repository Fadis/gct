#ifndef GCT_BUFFER_CREATE_INFO_HPP
#define GCT_BUFFER_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
namespace gct {
  class buffer_create_info_t : public chained_t {
  public:
    using self_type = buffer_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::BufferCreateInfo )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::BufferCollectionBufferCreateInfoFUCHSIA , buffer_collection )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::BufferDeviceAddressCreateInfoEXT , device_address )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_SETTER( vk::BufferOpaqueCaptureAddressCreateInfo , opaque_capture_address )
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::BufferOpaqueCaptureAddressCreateInfoKHR , opaque_capture_address )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::DedicatedAllocationBufferCreateInfoNV , dedicated_allocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::ExternalMemoryBufferCreateInfo , external_memory )
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::ExternalMemoryBufferCreateInfoKHR , external_memory )
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
  void to_json( nlohmann::json &root, const buffer_create_info_t &v );
  void from_json( const nlohmann::json &root, buffer_create_info_t &v );
}

#endif

