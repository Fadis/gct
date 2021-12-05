#include <gct/to_json.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/physical_device_memory_properties.hpp>
#include <gct/queue_family_properties.hpp>


#if defined(VK_VERSION_1_1) || defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#include <vulkan2json/PhysicalDeviceMemoryProperties.hpp>
#ifdef VK_VERSION_1_1
#include <vulkan2json/PhysicalDeviceMemoryProperties2.hpp>
#else
#include <vulkan2json/PhysicalDeviceMemoryProperties2KHR.hpp>
#endif
#else
#include <vulkan2json/PhysicalDeviceMemoryProperties.hpp>
#endif
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
#include <vulkan2json/PhysicalDeviceMemoryBudgetPropertiesEXT.hpp>
#endif
#include <vulkan2json/MemoryType.hpp>
#include <vulkan2json/MemoryHeap.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const physical_device_memory_properties_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( budget )
#endif
  }
  physical_device_memory_properties_t::physical_device_memory_properties_t(
    instance_t &instance,
    vk::PhysicalDevice &pdev,
    const extension_map_t &ext
  ) {
#if defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME) || defined(VK_VERSION_1_1)
    if(
      instance.get_api_version() >= VK_MAKE_VERSION( 1, 1, 0 ) ||
      instance.get_activated_extensions().find( "VK_KHR_get_physical_device_properties2" ) != instance.get_activated_extensions().end()
    ) {
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
      LIBGCT_EXTENSION_CREATE_IF_EXT( budget, VK_EXT_MEMORY_BUDGET_EXTENSION_NAME )
#endif
      rebuild_chain();
#ifdef VK_VERSION_1_1
      pdev.getMemoryProperties2(
        &basic
      );
#elif defined(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
      pdev.getMemoryProperties2KHR(
        &basic
      );
#endif
    }
    else {
      pdev.getMemoryProperties(
        &basic.memoryProperties
      );
    }
#else
    {
      pdev.getMemoryProperties(
        &basic
      );
    }
#endif
  }
  physical_device_memory_properties_t &physical_device_memory_properties_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
      LIBGCT_EXTENSION_REBUILD_CHAIN( budget )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

