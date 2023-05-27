#ifndef GCT_COMMAND_BUFFER_BEGIN_INFO_HPP
#define GCT_COMMAND_BUFFER_BEGIN_INFO_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class command_buffer_begin_info_t : public chained_t {
  public:
    using self_type = command_buffer_begin_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::CommandBufferBeginInfo )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupCommandBufferBeginInfo, device_group )
#elif defined(VK_STRUCTURE_TYPE_DEVICE_GROUP_PRESENT_CAPABILITIES_KHR)
    LIBGCT_EXTENSION_SETTER( vk::DeviceGroupCommandBufferBeginInfoKHR, device_group )
#endif
  };
}

#endif

