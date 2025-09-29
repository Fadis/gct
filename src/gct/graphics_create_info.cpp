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
#include <gct/mappable_buffer.hpp>

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
   const std::shared_ptr< scene_graph::scene_graph_resource > &r
) {
  LIBGCT_SET_SCENE_GRAPH_IMPL( r )
  return *this;
}

graphics_create_info &graphics_create_info::set_scene_graph(
   const scene_graph::scene_graph_resource &v
) {
  const auto r = &v;
  LIBGCT_SET_SCENE_GRAPH_IMPL( r )
  return *this;
}
graphics_create_info &graphics_create_info::use_dynamic_rendering(
  const std::vector< vk::Format > &color_attachment_format,
  vk::Format depth_attachment_format,
  vk::Format stencil_attachment_format
) {
  pipeline_create_info.use_dynamic_rendering(
    color_attachment_format,
    depth_attachment_format,
    stencil_attachment_format
  );
  return *this;
}
graphics_create_info &graphics_create_info::use_dynamic_rendering(
  vk::Format color_attachment_format,
  vk::Format depth_attachment_format,
  vk::Format stencil_attachment_format
) {
  pipeline_create_info.use_dynamic_rendering(
    color_attachment_format,
    depth_attachment_format,
    stencil_attachment_format
  );
  return *this;
}
graphics_create_info &graphics_create_info::use_color_blend( common_color_blend_mode mode ) {
  pipeline_create_info.use_color_blend( mode );
  return *this;
}
graphics_create_info &graphics_create_info::disable_depth_test() {
  pipeline_create_info.disable_depth_test();
  return *this;
}
graphics_create_info &graphics_create_info::disable_depth_write() {
  pipeline_create_info.disable_depth_write();
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

