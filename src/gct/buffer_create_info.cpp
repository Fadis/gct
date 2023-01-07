#include <gct/buffer_create_info.hpp>
#include <vulkan2json/BufferCreateInfo.hpp>
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
#include <vulkan2json/BufferCollectionBufferCreateInfoFUCHSIA.hpp>
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
#include <vulkan2json/BufferDeviceAddressCreateInfoEXT.hpp>
#endif
#ifdef VK_VERSION_1_2
#include <vulkan2json/BufferOpaqueCaptureAddressCreateInfo.hpp>
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
#include <vulkan2json/BufferOpaqueCaptureAddressCreateInfoKHR.hpp>
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
#include <vulkan2json/DedicatedAllocationBufferCreateInfoNV.hpp>
#endif
#ifdef VK_VERSION_1_1
#include <vulkan2json/ExternalMemoryBufferCreateInfo.hpp>
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
#include <vulkan2json/ExternalMemoryBufferCreateInfoKHR.hpp>
#endif
#ifdef VK_KHR_VIDEO_DECODE_H264_EXTENSION_NAME
#include <vulkan2json/VideoDecodeH264ProfileInfoKHR.hpp>
#elif defined(VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME)
#if VK_EXT_VIDEO_DECODE_H264_SPEC_VERSION < 6
#include <vulkan2json/VideoDecodeH264ProfileEXT.hpp>
#else
#include <vulkan2json/VideoDecodeH264ProfileInfoEXT.hpp>
#endif
#endif
#ifdef VK_KHR_VIDEO_DECODE_H265_EXTENSION_NAME
#include <vulkan2json/VideoDecodeH265ProfileInfoKHR.hpp>
#elif defined(VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME)
#if VK_EXT_VIDEO_DECODE_H265_SPEC_VERSION < 4
#include <vulkan2json/VideoDecodeH265ProfileEXT.hpp>
#else
#include <vulkan2json/VideoDecodeH265ProfileInfoEXT.hpp>
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
  void to_json( nlohmann::json &root, const buffer_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( buffer_collection )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( device_address )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_TO_JSON( opaque_capture_address )
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( opaque_capture_address )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( dedicated_allocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_TO_JSON( external_memory )
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video_profile )
    LIBGCT_EXTENSION_TO_JSON( video_profiles )
#endif
  }
  void from_json( const nlohmann::json &root, buffer_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to buffer_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_FUCHSIA_BUFFER_COLLECTION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( buffer_collection )
#endif
#ifdef VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( device_address )
#endif
#ifdef VK_VERSION_1_2
    LIBGCT_EXTENSION_FROM_JSON( opaque_capture_address )
#elif defined(VK_KHR_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( opaque_capture_address )
#endif
#ifdef VK_NV_DEDICATED_ALLOCATION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( dedicated_allocation )
#endif
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_FROM_JSON( external_memory )
#elif defined(VK_KHR_EXTERNAL_MEMORY_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( external_memory )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H264_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_h264_profile )
#endif
#ifdef VK_EXT_VIDEO_DECODE_H265_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_decode_h265_profile )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( video_profile )
    LIBGCT_EXTENSION_FROM_JSON( video_profiles )
#endif
  }
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

