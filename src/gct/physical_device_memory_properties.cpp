#include <gct/to_json.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/physical_device_properties.hpp>
#include <gct/physical_device_memory_properties.hpp>
#include <gct/queue_family_properties.hpp>

namespace gct {
  nlohmann::json to_json( const vk::MemoryType &v ) {
    auto root = nlohmann::json::object();
    root[ "propertyFlags" ] = std::uint32_t( v.propertyFlags );
    root[ "heapIndex" ] = v.heapIndex;
    return root;
  }
  nlohmann::json to_json( const vk::MemoryHeap &v ) {
    auto root = nlohmann::json::object();
    root[ "size" ] = v.size;
    root[ "flags" ] = std::uint32_t( v.flags );
    return root;
  }
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
  nlohmann::json to_json( const vk::PhysicalDeviceMemoryBudgetPropertiesEXT& v, std::uint32_t heap_count ) {
    auto root = nlohmann::json::object();
    root[ "heapBudget" ] = nlohmann::json::array();
    for( std::uint32_t i = 0; i != heap_count; ++i )
      root[ "heapBudget" ].push_back( v.heapBudget[ i ] );
    root[ "heapUsage" ] = nlohmann::json::array();
    for( std::uint32_t i = 0; i != heap_count; ++i )
      root[ "heapUsage" ].push_back( v.heapUsage[ i ] );
    return root;
  }
#endif
  nlohmann::json to_json( const vk::PhysicalDeviceMemoryProperties &v ) {
    auto root = nlohmann::json::object();
    root[ "memoryTypes" ] = nlohmann::json::array();
    for( std::uint32_t i = 0; i != v.memoryTypeCount; ++i )
      root[ "memoryTypes" ].push_back( to_json( v.memoryTypes[ i ] ) );
    root[ "memoryHeaps" ] = nlohmann::json::array();
    for( std::uint32_t i = 0; i != v.memoryHeapCount; ++i )
      root[ "memoryHeaps" ].push_back( to_json( v.memoryHeaps[ i ] ) );
    return root;
  }
  nlohmann::json to_json( const physical_device_memory_properties_t &v ) {
    auto root = nlohmann::json::object();
    root[ "basic" ] = to_json( v.get_basic() );
#ifdef VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
    if( v.has_budget() ) {
      root[ "budget" ] = to_json( v.get_budget(), v.get_basic().memoryHeapCount );
    }
#endif
    return root;
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

