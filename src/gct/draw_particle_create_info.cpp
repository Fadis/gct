#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/draw_particle_create_info.hpp>

namespace gct {

draw_particle_create_info &draw_particle_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
draw_particle_create_info &draw_particle_create_info::clear_resource() {
  resources.clear();
  return *this;
}
draw_particle_create_info &draw_particle_create_info::add_shader(
  const std::filesystem::path &p
) {
  shaders.push_back( p );
  return *this;
}
draw_particle_create_info &draw_particle_create_info::add_shader(
  const std::vector< std::filesystem::path > &p
) {
  for( const auto &e: p ) {
    shaders.push_back( e );
  }
  return *this;
}
draw_particle_create_info &draw_particle_create_info::clear_shader() {
  shaders.clear();
  return *this;
}

}

