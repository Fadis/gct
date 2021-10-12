#include <gct/to_json.hpp>
#include <gct/queue_family_properties.hpp>

namespace gct {
  nlohmann::json to_json( const vk::QueueFamilyProperties &v ) {
    auto root = nlohmann::json::object();
    root[ "queueFlags" ] = std::uint32_t( v.queueFlags );
    root[ "queueCount" ] = v.queueCount;
    root[ "timestampValidBits" ] = v.timestampValidBits;
    root[ "minImageTransferGranularity" ] = to_json( v.minImageTransferGranularity );
    return root;
  }

#if defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) && defined(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
  nlohmann::json to_json( const vk::QueueFamilyCheckpointProperties2NV &v ) {
    auto root = nlohmann::json::object();
    root[ "checkpointExecutionStageMask" ] = std::uint64_t( v.checkpointExecutionStageMask );
    return root;
  }
#endif

#ifdef VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
  nlohmann::json to_json( const vk::QueueFamilyCheckpointPropertiesNV &v ) {
    auto root = nlohmann::json::object();
    root[ "checkpointExecutionStageMask" ] = std::uint32_t( v.checkpointExecutionStageMask );
    return root;
  }
#endif

#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
  nlohmann::json to_json( const vk::QueueFamilyGlobalPriorityPropertiesEXT &v ) {
    auto root = nlohmann::json::object();
    root[ "priorities" ] = nlohmann::json::array();
    for( auto i = 0u; i != v.priorityCount; ++i )
      root[ "priorities" ].push_back( std::uint32_t( v.priorities[ i ] ) );
    return root;
  }
#endif

#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
  nlohmann::json to_json( const vk::VideoQueueFamilyProperties2KHR &v ) {
    auto root = nlohmann::json::object();
    root[ "videoCodecOperations" ] = std::uint32_t( v.videoCodecOperations );
    return root;
  }
#endif

  nlohmann::json to_json( const queue_family_properties_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.get_basic() );
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
    return root;
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

