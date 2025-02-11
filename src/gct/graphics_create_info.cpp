#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/shader_module.hpp>
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

void to_json( nlohmann::json &dest, const graphics_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shaders" ] = nlohmann::json::array();
  for( const auto &p: src.shaders ) {
    dest[ "shaders" ].push_back( p.string() );
  }
  dest[ "descriptor_set_layout" ] = nlohmann::json::array();
  for( const auto &p: src.descriptor_set_layout ) {
    if( p ) {
      dest[ "descriptor_set_layout" ].push_back( *p );
    }
    else {
      dest[ "descriptor_set_layout" ].push_back( nullptr );
    }
  }
  dest[ "shader_module" ] = nlohmann::json::array();
  for( const auto &p: src.shader_module ) {
    if( p ) {
      dest[ "shader_module" ].push_back( *p );
    }
    else {
      dest[ "shader_module" ].push_back( nullptr );
    }
  }
  dest[ "pipeline_create_info" ] = src.pipeline_create_info;
  dest[ "external_descriptor_set" ] = nlohmann::json::object();
  for( const auto &p: src.external_descriptor_set ) {
    if( p.second ) {
      dest[ "external_descriptor_set" ][ std::to_string( p.first ) ] = *p.second;
    }
    else {
      dest[ "external_descriptor_set" ][ std::to_string( p.first ) ] = nullptr;
    }
  }
  dest[ "resources" ] = src.resources;
  dest[ "swapchain_image_count" ] = src.swapchain_image_count;
  dest[ "ignore_unused_descriptor" ] = src.ignore_unused_descriptor;
}

}

