#include <nlohmann/json.hpp>
#include <vulkan2json/Format.hpp>
#include <gct/gbuffer_create_info.hpp>
#include <gct/allocator.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const gbuffer_create_info &src ) {
  dest = nlohmann::json::object();
  if( src.allocator ) {
    dest[ "allocator" ] = src.allocator->get_props();
  }
  else {
    dest[ "allocator" ] = nullptr;
  }
  dest[ "width" ] = src.width;
  dest[ "height" ] = src.height;
  dest[ "layer" ] = src.layer;
  dest[ "swapchain_image_count" ] = src.swapchain_image_count;
  dest[ "color_buffer_count" ] = src.color_buffer_count;
  dest[ "has_depth" ] = src.has_depth;
  dest[ "format" ] = src.format;
}

}

