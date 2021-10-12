#include <algorithm>
#include <iterator>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>

namespace gct {
  extension_map_t
  get_device_extensions(
    const vk::PhysicalDevice &pdev,
    const std::vector< const char* > &dlayers
  ) {
    auto base = pdev.enumerateDeviceExtensionProperties( nullptr );
    for( const auto &layer: dlayers ) {
      auto additional = pdev.enumerateDeviceExtensionProperties( std::string( layer ) );
      base.insert( base.end(), additional.begin(), additional.end() );
    }
    extension_map_t hashed;
    std::transform(
      base.begin(),
      base.end(),
      std::inserter( hashed, hashed.end() ),
      []( const auto &v ) {
        return std::make_pair(
          v.extensionName,
          extension_version_t( v.specVersion )
        );
      }
    );
    return hashed;
  }
}

