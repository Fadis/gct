#include <nlohmann/json.hpp>
#include <gct/gbuffer.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/draw_particle_create_info.hpp>

namespace gct {

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

void to_json( nlohmann::json &dest, const draw_particle_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  if( src.output ) {
    dest[ "output" ] = *src.output;
  }
  dest[ "shaders" ] = nlohmann::json::array();
  for( const auto &p: src.shaders ) {
    dest[ "shaders" ].push_back( p.string() );
  }
  dest[ "resources" ] = src.resources;
}

}

