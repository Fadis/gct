#ifndef LIBGCT_INCLUDE_GCT_QUEUE_REQUIREMENT_HPP
#define LIBGCT_INCLUDE_GCT_QUEUE_REQUIREMENT_HPP
#include <optional>
#include <vulkan/vulkan.hpp>

namespace gct {
  struct surface_t;
  struct queue_requirement_t {
    vk::QueueFlags flags = vk::QueueFlags( 0 );
    std::uint32_t timestamp_valid_bits = 0u;
    vk::Extent3D image_transfer_granularity;
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
    std::optional< vk::QueueGlobalPriorityEXT > global_priority;
#endif
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
    std::vector< vk::SurfaceKHR > surface;
#endif
    vk::CommandPoolCreateFlagBits command_pool_create_flags = vk::CommandPoolCreateFlagBits( 0u );
  };
}

#endif

