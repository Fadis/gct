#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/occlusion_query_create_info.hpp>

namespace gct {

occlusion_query_create_info &occlusion_query_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
occlusion_query_create_info &occlusion_query_create_info::clear_resource() {
  resources.clear();
  return *this;
}
occlusion_query_create_info &occlusion_query_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( dir / "world.vert.spv" );
  add_shader( dir / "world.geom.spv" );
  add_shader( dir / "world.frag.spv" );
  return *this;
}
occlusion_query_create_info &occlusion_query_create_info::add_shader(
  const std::filesystem::path &p
) {
  shaders.push_back( p );
  return *this;
}
occlusion_query_create_info &occlusion_query_create_info::add_shader(
  const std::vector< std::filesystem::path > &p
) {
  for( const auto &e: p ) {
    shaders.push_back( e );
  }
  return *this;
}
occlusion_query_create_info &occlusion_query_create_info::clear_shader() {
  shaders.clear();
  return *this;
}

}

