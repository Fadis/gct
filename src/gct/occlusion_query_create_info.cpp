#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
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

void to_json( nlohmann::json &dest, const occlusion_query_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "color_attachment_count" ] = src.color_attachment_count;
  if( src.depth_image ) {
    dest[ "depth_image" ] = *src.depth_image;
  }
  dest[ "shaders" ] = nlohmann::json::array();
  for( const auto &p : src.shaders ) {
    dest[ "shaders" ].push_back( p.string() );
  }
  dest[ "dynamic_cull_mode" ] = src.dynamic_cull_mode;
  dest[ "max_query_count" ] = src.max_query_count;
  dest[ "query" ] = src.query;
  dest[ "resources" ] = src.resources;
}

}

