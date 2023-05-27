#ifndef LIBGCT_INCLUDE_GCT_TO_JSON_HPP
#define LIBGCT_INCLUDE_GCT_TO_JSON_HPP
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>

namespace gct {
#ifdef VK_KHR_SURFACE_EXTENSION_NAME
  void to_json( nlohmann::json&, const std::vector< vk::SurfaceFormatKHR > &v );
#endif
}

#endif

