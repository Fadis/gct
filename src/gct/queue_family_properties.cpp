#include <gct/to_json.hpp>
#include <gct/queue_family_properties.hpp>
#include <vulkan2json/QueueFamilyProperties.hpp>
#ifdef VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
#include <vulkan2json/QueueFamilyCheckpointPropertiesNV.hpp>
#endif
#if defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) && defined(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
#include <vulkan2json/QueueFamilyCheckpointProperties2NV.hpp>
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
#include <vulkan2json/QueueFamilyGlobalPriorityPropertiesEXT.hpp>
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
#include <vulkan2json/VideoQueueFamilyProperties2KHR.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const queue_family_properties_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( checkpoint )
#endif
#if defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) && defined(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( checkpoint2 )
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( global_priority )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( video )
#endif
  }

  queue_family_properties_t &queue_family_properties_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( checkpoint )
#endif
#if defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) && defined(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( checkpoint2 )
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( global_priority )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( video )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  queue_family_properties_t &queue_family_properties_t::activate( const extension_map_t &ext ) {
#ifdef VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
    LIBGCT_EXTENSION_CREATE_IF_EXT( checkpoint, "VK_NV_device_diagnostic_checkpoints" )
#endif
#if defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) && defined(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
    if(
      ext.find( "VK_NV_device_diagnostic_checkpoints" ) != ext.end() &&
      ext.find( "VK_KHR_synchronization2" ) != ext.end()
    ) {
      checkpoint2.reset( new checkpoint2_t() );
    }
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_CREATE_IF_EXT( global_priority, "VK_EXT_global_priority_query" )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
    LIBGCT_EXTENSION_CREATE_IF_EXT( video, "VK_KHR_video_queue" )
    if( video ) {
      // avoid invalid validation
      video->videoCodecOperations = vk::VideoCodecOperationFlagBitsKHR::eEncodeH264EXT;
    }
#endif
    rebuild_chain();
    return *this;
  }
}

