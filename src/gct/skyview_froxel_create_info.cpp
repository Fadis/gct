#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/skyview_froxel_create_info.hpp>

namespace gct {

skyview_froxel_create_info &skyview_froxel_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
skyview_froxel_create_info &skyview_froxel_create_info::clear_resource() {
  resources.clear();
  return *this;
}
skyview_froxel_create_info &skyview_froxel_create_info::set_shader(
  const std::filesystem::path &dir
) {
  set_generate_shader( dir / "froxel.comp.spv" );
  set_render_shader( dir / "render_froxel.comp.spv" );
  return *this;
}

}

