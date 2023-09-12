#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/epipolar_light_scattering_create_info.hpp>

namespace gct {

epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::clear_resource() {
  resources.clear();
  return *this;
}
epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::add_draw_mesh_shader(
  const std::filesystem::path &p
) {
  draw_mesh_shaders.push_back( p );
  return *this;
}
epipolar_light_scattering_create_info &epipolar_light_scattering_create_info::clear_draw_mesh_shader() {
  draw_mesh_shaders.clear();
  return *this;
}

}

