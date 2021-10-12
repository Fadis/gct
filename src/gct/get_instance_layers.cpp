#include <iostream>
#include <algorithm>
#include <iterator>
#include <vulkan/vulkan.hpp>
#include <gct/get_extensions.hpp>

namespace gct {
  layer_map_t
  get_instance_layers() {
    auto base = vk::enumerateInstanceLayerProperties();
    layer_map_t hashed;
    std::transform(
      base.begin(),
      base.end(),
      std::inserter( hashed, hashed.end() ),
      []( const auto &v ) {
        return std::make_pair(
          v.layerName,
          v
        );
      }
    );
    return hashed;
  }
}

