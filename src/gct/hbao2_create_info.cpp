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
  add_shader( "sum", dir / "hbao.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const hbao2_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader;
  dest[ "input_name" ] = src.input_name;
  dest[ "partial0_name" ] = src.partial0_name;
  dest[ "partial45_name" ] = src.partial45_name;
  dest[ "partial90_name" ] = src.partial90_name;
  dest[ "partial135_name" ] = src.partial135_name;
  dest[ "output_name" ] = src.output_name;
  dest[ "resources" ] = src.resources;
  dest[ "scene_graph" ] = *src.scene_graph;
}

}

