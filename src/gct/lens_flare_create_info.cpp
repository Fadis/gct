#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/named_resource.hpp>
#include <gct/lens_flare_create_info.hpp>

namespace gct {

lens_flare_create_info &lens_flare_create_info::add_resource(
  const named_resource &n
) {
  resources.push_back( n );
  return *this;
}
lens_flare_create_info &lens_flare_create_info::clear_resource() {
  resources.clear();
  return *this;
}
lens_flare_create_info &lens_flare_create_info::set_shader(
  const std::filesystem::path &dir
) {
  add_shader( dir / "world.vert.spv" );
  add_shader( dir / "world.geom.spv" );
  add_shader( dir / "world.frag.spv" );
  return *this;
}
lens_flare_create_info &lens_flare_create_info::add_shader(
  const std::filesystem::path &p
) {
  shaders.push_back( p );
  return *this;
}
lens_flare_create_info &lens_flare_create_info::add_shader(
  const std::vector< std::filesystem::path > &p
) {
  for( const auto &e: p ) {
    shaders.push_back( e );
  }
  return *this;
}
lens_flare_create_info &lens_flare_create_info::clear_shader() {
  shaders.clear();
  return *this;
}

void to_json( nlohmann::json &dest, const lens_flare_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "descriptor_set_layout" ] = nlohmann::json::array();
  for( const auto &p: src.descriptor_set_layout ) {
    if( p ) {
      dest[ "descriptor_set_layout" ].push_back( *p );
    }
    else {
      dest[ "descriptor_set_layout" ].push_back( nullptr );
    }
  }
  dest[ "shaders" ] = nlohmann::json::array();
  for( const auto &p: src.shaders ) {
    dest[ "shaders" ].push_back( p.string() );
  }
  dest[ "width" ] = src.width;
  dest[ "height" ] = src.height;
  if( src.texture ) {
    dest[ "texture" ] = *src.texture;
  }
  dest[ "lens_size" ] = src.lens_size;
  dest[ "sensor_size" ] = nlohmann::json::array();
  dest[ "sensor_size" ].push_back( src.sensor_size[ 0 ] );
  dest[ "sensor_size" ].push_back( src.sensor_size[ 1 ] );
  dest[ "matrix_count" ] = src.matrix_count;
  dest[ "external_descriptor_set" ] = nlohmann::json::object();
  for( const auto &v: src.external_descriptor_set ) {
    if( v.second ) {
      dest[ "external_descriptor_set" ][ std::to_string( v.first ) ] = *v.second;
    }
    else {
      dest[ "external_descriptor_set" ][ std::to_string( v.first ) ] = nullptr;
    }
  }
  dest[ "resources" ] = src.resources;
}

}

