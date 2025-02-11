#include <nlohmann/json.hpp>
#include <gct/image_view.hpp>
#include <gct/named_resource.hpp>
#include <gct/rtao_create_info.hpp>

namespace gct {

rtao_create_info &rtao_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_hgauss_shader( dir / "hadaptive.comp.spv" );
  set_vgauss_shader( dir / "vadaptive.comp.spv" );
  set_rtao_shader( dir / "rtao.comp.spv" );
  return *this;
}

void to_json( nlohmann::json &dest, const rtao_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "hgauss_shader" ] = src.hgauss_shader.string();
  dest[ "vgauss_shader" ] = src.vgauss_shader.string();
  dest[ "rtao_shader" ] = src.rtao_shader.string();
  dest[ "previous_ao_name" ] = src.previous_ao_name;
  dest[ "previous_history_name" ] = src.previous_history_name;
  dest[ "history_name" ] = src.history_name;
  dest[ "temporary_name" ] = src.temporary_name;
  dest[ "ao_name" ] = src.ao_name;
  if( src.output ) {
    dest[ "output" ] = *src.output;
  }
  dest[ "resources" ] = src.resources;
}

}

