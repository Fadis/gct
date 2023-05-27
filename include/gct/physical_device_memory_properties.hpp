#ifndef LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_MEMORY_PROPERTIES_HPP
#define LIBGCT_INCLUDE_GCT_PHYSICAL_DEVICE_MEMORY_PROPERTIES_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>

namespace gct {
  class instance_t;
  class physical_device_memory_properties_t : public chained_t {
public:
    physical_device_memory_properties_t(
      instance_t &instance,
      vk::PhysicalDevice &pdev,
      const extension_map_t &ext
    );
    using self_type = physical_device_memory_properties_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
private:
    using basic_t = vk::PhysicalDeviceMemoryProperties;
#ifdef VK_VERSION_1_1
    using basic2_t = vk::PhysicalDeviceMemoryProperties2;
#else
    using basic2_t = vk::PhysicalDeviceMemoryProperties2KHR;
#endif
    basic2_t basic;
public:
    LIBGCT_BASIC_SETTER_INDIRECT( memoryProperties )
#else
    LIBGCT_BASIC_SETTER( vk::PhysicalDeviceMemoryProperties )
#endif
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PhysicalDeviceMemoryBudgetPropertiesEXT , budget )
#endif
  };
  void to_json( nlohmann::json&, const physical_device_memory_properties_t &v );
}

#endif
