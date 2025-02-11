#include <nlohmann/json.hpp>
#include <gct/named_resource.hpp>
#include <gct/distance_field2_create_info.hpp>

namespace gct {

distance_field2_create_info &distance_field2_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
distance_field2_create_info &distance_field2_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}
distance_field2_create_info &distance_field2_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_voronoi_shader( dir / "voronoi.comp.spv" );
  set_distance_field_shader( dir / "distance_field.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const distance_field2_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "voronoi_shader" ] = src.voronoi_shader.string();
  dest[ "distance_field_shader" ] = src.distance_field_shader.string();
  dest[ "size_factor" ] = src.size_factor;
  dest[ "resources" ] = src.resources;
}

}

