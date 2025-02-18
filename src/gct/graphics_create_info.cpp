#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/shader_module.hpp>
#include <gct/named_resource.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/light_pool.hpp>
#include <gct/graphics_create_info.hpp>

namespace gct {

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

graphics_create_info &graphics_create_info::set_scene_graph(
   const scene_graph::scene_graph_resource &r
) {
  {
    const auto iter = r.descriptor_set_layout.find( r.image_descriptor_set_id );
    if( iter != r.descriptor_set_layout.end() && r.image_descriptor_set ) {
      descriptor_set_layout[ r.image_descriptor_set_id ] = iter->second;
      external_descriptor_set[ r.image_descriptor_set_id ] = r.image_descriptor_set;
    }
  }
  {
    const auto iter = r.descriptor_set_layout.find( r.texture_descriptor_set_id );
    if( iter != r.descriptor_set_layout.end() && r.texture_descriptor_set ) {
      descriptor_set_layout[ r.texture_descriptor_set_id ] = iter->second;
      external_descriptor_set[ r.texture_descriptor_set_id ] = r.texture_descriptor_set;
    }
  }
  if( r.matrix ) {
    add_resource( { "matrix_pool", r.matrix->get_buffer() } );
  }
  if( r.aabb ) {
    add_resource( { "aabb_pool", r.aabb->get_buffer() } );
  }
  if( r.primitive_resource_index ) {
    add_resource( { "primitive_resource_index", r.primitive_resource_index->get_buffer() } );
  }
  if( r.instance_resource_index ) {
    add_resource( { "instance_resource_index", r.instance_resource_index->get_buffer() } );
  }
  if( r.visibility ) {
    add_resource( { "visibility", r.visibility->get_buffer() } );
  }
  if( r.light ) {
    add_resource( { "light_pool", r.light->get_buffer() } );
  }
  ignore_unused_descriptor = true;
  return *this;
}

void to_json( nlohmann::json &dest, const graphics_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shaders" ] = nlohmann::json::array();
  for( const auto &p: src.shaders ) {
    dest[ "shaders" ].push_back( p.string() );
  }
  dest[ "descriptor_set_layout" ] = nlohmann::json::object();
  for( const auto &v: src.descriptor_set_layout ) {
    if( v.second ) {
      dest[ "descriptor_set_layout" ][ std::to_string( v.first ) ] = *v.second;
    }
    else {
      dest[ "descriptor_set_layout" ][ std::to_string( v.first ) ] = nullptr;
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

