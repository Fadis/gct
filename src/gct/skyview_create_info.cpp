#include <vulkan2json/Format.hpp>
#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/skyview_create_info.hpp>

namespace gct {

skyview_create_info &skyview_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_transmittance_shader( dir / "generate_transmittance.comp.spv" );
  set_multiscat_shader( dir / "generate_multiscat.comp.spv" );
  set_skyview_shader( dir / "generate_skyview.comp.spv" );
  return *this;
}

bool skyview_create_info::shader_exists() const {
  if( !std::filesystem::exists( transmittance_shader ) ) return false;
  if( !std::filesystem::exists( multiscat_shader ) ) return false;
  if( !std::filesystem::exists( skyview_shader ) ) return false;
  return true;
}

void to_json( nlohmann::json &dest, const skyview_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "transmittance_width" ] = src.transmittance_width;
  dest[ "transmittance_height" ] = src.transmittance_height;
  dest[ "multiscat_size" ] = src.multiscat_size;
  dest[ "skyview_width" ] = src.skyview_width;
  dest[ "skyview_height" ] = src.skyview_height;
  dest[ "format" ] = src.format;
  dest[ "transmittance_shader" ] = src.transmittance_shader.string();
  dest[ "multiscat_shader" ] = src.multiscat_shader.string();
  dest[ "skyview_shader" ] = src.skyview_shader.string();
  dest[ "resources" ] = src.resources;
}

}

