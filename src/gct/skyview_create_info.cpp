#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/skyview_create_info.hpp>

namespace gct {

skyview_create_info &skyview_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
skyview_create_info &skyview_create_info::clear_resource() {
  resources.clear();
  return *this;
}
skyview_create_info &skyview_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_transmittance_shader( dir / "generate_transmittance.comp.spv" );
  set_multiscat_shader( dir / "generate_multiscat.comp.spv" );
  set_skyview_shader( dir / "generate_skyview.comp.spv" );
  return *this;
}

}

