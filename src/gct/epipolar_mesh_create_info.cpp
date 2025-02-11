#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/epipolar_mesh_create_info.hpp>

namespace gct {

epipolar_mesh_create_info &epipolar_mesh_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
epipolar_mesh_create_info &epipolar_mesh_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const epipolar_mesh_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "pole_count" ] = src.pole_count;
  dest[ "vertex_count" ] = src.vertex_count;
  dest[ "generate2_shader" ] = src.generate2_shader.string();
  dest[ "generate3_shader" ] = src.generate3_shader.string();
  dest[ "generate4_shader" ] = src.generate4_shader.string();
  dest[ "attrs" ] = nlohmann::json::object();
  for( const auto &a: src.attrs ) {
    dest[ "attrs" ][ to_string( a.first ) ] = a.second;
  }
  dest[ "stride" ] = src.stride;
  dest[ "swapchain_image_count" ] = src.swapchain_image_count;
  dest[ "resources" ] = src.resources;
}

}


