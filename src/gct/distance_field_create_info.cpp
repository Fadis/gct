#include <nlohmann/json.hpp>
#include <gct/named_resource.hpp>
#include <gct/distance_field_create_info.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const distance_field_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "clear_shader" ] = src.clear_shader.string();
  dest[ "voronoi_shader" ] = src.voronoi_shader.string();
  dest[ "distance_field_shader" ] = src.distance_field_shader.string();
  dest[ "size_factor" ] = src.size_factor;
}

}

