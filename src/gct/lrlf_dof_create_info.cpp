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
  add_shader( "v0", dir / "h0.comp.spv" );
  add_shader( "v1", dir / "h1.comp.spv" );
  add_shader( "v2", dir / "h2.comp.spv" );
  add_shader( "v3", dir / "h3.comp.spv" );
  add_shader( "v4", dir / "h4.comp.spv" );
  add_shader( "mix", dir / "mix.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const lrlf_dof_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader_set" ] = src.shader_set;
  dest[ "rename" ] = src.rename;
  dest[ "resources" ] = src.resources;
  dest[ "scene_graph" ] = *src.scene_graph;
}

}

