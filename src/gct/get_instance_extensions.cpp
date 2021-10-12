#include <algorithm>
#include <iterator>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>

namespace gct {
  extension_map_t
  get_instance_extensions( const std::vector< const char* > &ilayers ) {
    auto base = vk::enumerateInstanceExtensionProperties( nullptr );
    for( const auto &layer: ilayers ) {
      auto additional = vk::enumerateInstanceExtensionProperties( std::string( layer ) );
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

