#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/hbao2_create_info.hpp>
#include <gct/scene_graph_resource.hpp>

namespace gct {

hbao2_create_info &hbao2_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( "0", dir / "hbao_0.comp.spv" );
  add_shader( "45", dir / "hbao_45.comp.spv" );
  add_shader( "90", dir / "hbao_90.comp.spv" );
  add_shader( "135", dir / "hbao_135.comp.spv" );
  add_shader( "mix", dir / "hbao.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const hbao2_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader_set" ] = src.shader_set;
  dest[ "rename" ] = src.rename;
  dest[ "resources" ] = src.resources;
  dest[ "scene_graph" ] = *src.scene_graph;
  dest[ "node_name" ] = src.node_name;
}

}

