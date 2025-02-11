#include <nlohmann/json.hpp>
#include <gct/buffer.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/hierarchical_raster_occlusion_culling_create_info.hpp>

namespace gct {

hierarchical_raster_occlusion_culling_create_info &hierarchical_raster_occlusion_culling_create_info::set_shader(
  const std::filesystem::path &dir
) {
  sort.set_shader( dir );
  set_setup_sort_shader( dir / "setup_sort.comp.spv" );
  set_setup_leaf_shader( dir / "setup_leaf.comp.spv" );
  set_setup_node_shader( dir / "setup_node.comp.spv" );
  set_mark_leaf_shader( dir / "mark_leaf.comp.spv" );
  set_mark_node_shader( dir / "mark_node.comp.spv" );
  return *this;
}
hierarchical_raster_occlusion_culling_create_info &hierarchical_raster_occlusion_culling_create_info::add_shader(
    const std::filesystem::path &p
) {
  shaders.push_back( p );
  return *this;
}

hierarchical_raster_occlusion_culling_create_info &hierarchical_raster_occlusion_culling_create_info::add_shader(
    const std::vector< std::filesystem::path > &p
) {
  shaders.insert( shaders.end(), p.begin(), p.end() );
  return *this;
}

hierarchical_raster_occlusion_culling_create_info &hierarchical_raster_occlusion_culling_create_info::clear_shader() {
  shaders.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const hierarchical_raster_occlusion_culling_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "setup_sort_shader" ] = src.setup_sort_shader;
  dest[ "sort" ] = src.sort;
  dest[ "setup_leaf_shader" ] = src.setup_leaf_shader;
  dest[ "setup_node_shader" ] = src.setup_node_shader;
  dest[ "mark_leaf_shader" ] = src.mark_leaf_shader;
  dest[ "mark_node_shader" ] = src.mark_node_shader;
  dest[ "shaders" ] = src.shaders;
  dest[ "resources" ] = src.resources;
}

}

