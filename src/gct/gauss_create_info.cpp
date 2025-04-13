#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/gauss_create_info.hpp>
#include <gct/scene_graph_resource.hpp>

namespace gct {

gauss_create_info &gauss_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( "horizontal", dir / "horizontal.comp.spv" );
  add_shader( "vertical", dir / "vertical.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const gauss_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader_set" ] = src.shader_set;
  dest[ "rename" ] = src.rename;
  dest[ "resources" ] = src.resources;
  dest[ "scene_graph" ] = *src.scene_graph;
  dest[ "node_name" ] = src.node_name;
  dest[ "scale" ] = src.scale;
}

}

