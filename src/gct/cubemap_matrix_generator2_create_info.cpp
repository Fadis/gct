#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/named_resource.hpp>
#include <gct/cubemap_matrix_generator2_create_info.hpp>
#include <gct/scene_graph_resource.hpp>

namespace gct {

cubemap_matrix_generator2_create_info &cubemap_matrix_generator2_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( "generate", dir / "generate.comp.spv" );
  return *this;
}

bool cubemap_matrix_generator2_create_info::shader_exists() const {
  if( shader_set.find( "generate" ) == shader_set.end() ) return false; 
  if( !std::filesystem::exists( shader_set.at( "generate" ) ) ) return false;
  return true;
}

void to_json( nlohmann::json &dest, const cubemap_matrix_generator2_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader_set" ] = src.shader_set;
  dest[ "resources" ] = src.resources;
  dest[ "scene_graph" ] = *src.scene_graph;
  dest[ "node_name" ] = src.node_name;
}

}

