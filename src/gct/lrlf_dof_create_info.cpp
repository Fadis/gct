#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/lrlf_dof_create_info.hpp>
#include <gct/scene_graph_resource.hpp>

namespace gct {

lrlf_dof_create_info &lrlf_dof_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( "h0", dir / "h0.comp.spv" );
  add_shader( "h1", dir / "h1.comp.spv" );
  add_shader( "h2", dir / "h2.comp.spv" );
  add_shader( "h3", dir / "h3.comp.spv" );
  add_shader( "h4", dir / "h4.comp.spv" );
  add_shader( "v", dir / "v.comp.spv" );
  return *this;
}

bool lrlf_dof_create_info::shader_exists() const {
  if( shader_set.find( "h0" ) == shader_set.end() ) return false; 
  if( shader_set.find( "h1" ) == shader_set.end() ) return false; 
  if( shader_set.find( "h2" ) == shader_set.end() ) return false; 
  if( shader_set.find( "h3" ) == shader_set.end() ) return false; 
  if( shader_set.find( "h4" ) == shader_set.end() ) return false; 
  if( shader_set.find( "v" ) == shader_set.end() ) return false;
  if( !std::filesystem::exists( shader_set.at( "h0" ) ) ) return false;
  if( !std::filesystem::exists( shader_set.at( "h1" ) ) ) return false;
  if( !std::filesystem::exists( shader_set.at( "h2" ) ) ) return false;
  if( !std::filesystem::exists( shader_set.at( "h3" ) ) ) return false;
  if( !std::filesystem::exists( shader_set.at( "h4" ) ) ) return false;
  if( !std::filesystem::exists( shader_set.at( "v" ) ) ) return false;
  return true;
}

void to_json( nlohmann::json &dest, const lrlf_dof_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader_set" ] = src.shader_set;
  dest[ "rename" ] = src.rename;
  dest[ "resources" ] = src.resources;
  dest[ "scene_graph" ] = *src.scene_graph;
  dest[ "node_name" ] = src.node_name;
}

}

