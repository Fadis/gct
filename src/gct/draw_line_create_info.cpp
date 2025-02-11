#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/draw_line_create_info.hpp>

namespace gct {

draw_line_create_info &draw_line_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( dir / "world.vert.spv" );
  if( std::filesystem::exists( dir / "world.geom.spv" ) ) {
    add_shader( dir / "world.geom.spv" );
  }
  add_shader( dir / "world.frag.spv" );
  return *this;
}
draw_line_create_info &draw_line_create_info::add_shader(
  const std::filesystem::path &p
) {
  shaders.push_back( p );
  return *this;
}
draw_line_create_info &draw_line_create_info::add_shader(
  const std::vector< std::filesystem::path > &p
) {
  for( const auto &e: p ) {
    shaders.push_back( e );
  }
  return *this;
}
draw_line_create_info &draw_line_create_info::clear_shader() {
  shaders.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const draw_line_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "color_attachment_count" ] = src.color_attachment_count;
  dest[ "shaders" ] = nlohmann::json::array();
  for( const auto &p: src.shaders ) {
    dest[ "shaders" ].push_back( p.string() );
  }
  dest[ "dynamic_cull_mode" ] = src.dynamic_cull_mode;
  dest[ "max_line_count" ] = src.max_line_count;
  dest[ "line_width" ] = src.line_width;
  dest[ "width" ] = src.width;
  dest[ "height" ] = src.height;
  dest[ "resources" ] = src.resources;
}

}

