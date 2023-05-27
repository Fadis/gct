#ifndef LIBGCT_INCLUDE_GCT_QUEUE_FAMILY_PROPERTIES_HPP
#define LIBGCT_INCLUDE_GCT_QUEUE_FAMILY_PROPERTIES_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>
#include <gct/extension.hpp>

namespace gct {
  struct queue_family_properties_t : public chained_t {
    using self_type = queue_family_properties_t;
    self_type &activate( const extension_map_t& );
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
private:
    using basic_t = vk::QueueFamilyProperties;
#ifdef VK_VERSION_1_1
    using basic2_t = vk::QueueFamilyProperties2;
#else
    using basic2_t = vk::QueueFamilyProperties2KHR;
#endif
    basic2_t basic; 
public:
    LIBGCT_BASIC_SETTER_INDIRECT( queueFamilyProperties )
#else
    LIBGCT_BASIC_SETTER( vk::QueueFamilyProperties )
#endif
#ifdef VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::QueueFamilyCheckpointPropertiesNV , checkpoint )
#endif
#if defined(VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME) && defined(VK_NV_DEVICE_DIAGNOSTIC_CHECKPOINTS_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::QueueFamilyCheckpointProperties2NV , checkpoint2 )
#endif
#ifdef VK_EXT_GLOBAL_PRIORITY_QUERY_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::QueueFamilyGlobalPriorityPropertiesEXT , global_priority )
#endif
#ifdef VK_KHR_VIDEO_QUEUE_EXTENSION_NAME
#if VK_KHR_VIDEO_QUEUE_SPEC_VERSION < 5
    LIBGCT_EXTENSION_SETTER( vk::VideoQueueFamilyProperties2KHR , video )
#else
    LIBGCT_EXTENSION_SETTER( vk::QueueFamilyVideoPropertiesKHR , video )
#endif
#endif
  };
  void to_json( nlohmann::json&, const queue_family_properties_t& );
}

#endif

