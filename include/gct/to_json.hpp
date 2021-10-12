#ifndef LIBGCT_INCLUDE_GCT_TO_JSON_HPP
#define LIBGCT_INCLUDE_GCT_TO_JSON_HPP
#include <nlohmann/json.hpp>
#include <vulkan/vulkan.hpp>

namespace gct {
  nlohmann::json to_json( const vk::Extent3D &v );
  nlohmann::json to_json( const vk::Extent2D &v );
  nlohmann::json to_json( const vk::Offset2D &v );
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  nlohmann::json to_json( const vk::SurfaceFormatKHR &v );
  nlohmann::json to_json( const std::vector< vk::SurfaceFormatKHR > &v );
#endif
}

#endif

