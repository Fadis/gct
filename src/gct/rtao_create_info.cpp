#include <gct/named_resource.hpp>
#include <gct/rtao_create_info.hpp>

namespace gct {

rtao_create_info &rtao_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
rtao_create_info &rtao_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}
rtao_create_info &rtao_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_hgauss_shader( dir / "hadaptive.comp.spv" );
  set_vgauss_shader( dir / "vadaptive.comp.spv" );
  set_rtao_shader( dir / "rtao.comp.spv" );
  return *this;
}

}

