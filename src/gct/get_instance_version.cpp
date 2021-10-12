#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>

namespace gct {
  std::uint32_t get_instance_version() {
#ifdef VK_VERSION_1_1
    return vk::enumerateInstanceVersion();
#else
    return VK_API_VERSION_1_0;
#endif
  }
}

