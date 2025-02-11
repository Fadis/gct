#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/skyview_froxel_create_info.hpp>

namespace gct {

skyview_froxel_create_info &skyview_froxel_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_generate_shader( dir / "froxel.comp.spv" );
  set_render_shader( dir / "render_froxel.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const skyview_froxel_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "froxel_xy_resolution" ] = src.froxel_xy_resolution;
  dest[ "froxel_z_resolution" ] = src.froxel_z_resolution;
  dest[ "width" ] = src.width;
  dest[ "height" ] = src.height;
  dest[ "generate_shader" ] = src.generate_shader.string();
  dest[ "render_shader" ] = src.render_shader.string();
  if( src.gbuffer ) {
    dest[ "gbuffer" ] = *src.gbuffer;
  }
  if( src.transmittance ) {
    dest[ "transmittance" ] = *src.transmittance;
  }
  if( src.multiscat ) {
    dest[ "multiscat" ] = *src.multiscat;
  }
  dest[ "gbuffer_name" ] = src.gbuffer_name;
  dest[ "transmittance_name" ] = src.transmittance_name;
  dest[ "multiscat_name" ] = src.multiscat_name;
  dest[ "froxel_name" ] = src.froxel_name;
  dest[ "froxel_state_name" ] = src.froxel_state_name;
  dest[ "output_name" ] = src.output_name;
  dest[ "resources" ] = src.resources;
}

}

