#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/graphics_create_info.hpp>

namespace gct {

graphics_create_info &graphics_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
graphics_create_info &graphics_create_info::clear_resource(
  const named_resource &n
) {
  resources.clear();
  return *this;
}
graphics_create_info &graphics_create_info::add_shader(
  const std::filesystem::path &p
) {
  shaders.push_back( p );
  return *this;
}
graphics_create_info &graphics_create_info::add_shader(
  const std::vector< std::filesystem::path > &p
) {
  for( const auto &e: p ) {
    shaders.push_back( e );
  }
  return *this;
}
graphics_create_info &graphics_create_info::add_shader(
  const std::shared_ptr< shader_module_t > &p
) {
  shader_module.push_back( p );
  return *this;
}
graphics_create_info &graphics_create_info::clear_shader() {
  shaders.clear();
  shader_module.clear();
  return *this;
}

}

