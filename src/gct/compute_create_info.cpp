#include <nlohmann/json.hpp>
#include <gct/named_resource.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/compute_create_info.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/light_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/specialization_map.hpp>

namespace gct {
compute_create_info &compute_create_info::set_scene_graph(
   const std::shared_ptr< scene_graph::scene_graph_resource > &r
) {
  LIBGCT_SET_SCENE_GRAPH_IMPL( r )
  return *this;
}

void to_json( nlohmann::json &dest, const compute_create_info &src ) {
  dest = nlohmann::json::object();
  dest[ "allocator_set" ] = src.allocator_set;
  dest[ "shader" ] = src.shader.string();
  dest[ "descriptor_set_layout" ] = nlohmann::json::object();
  for( const auto &v: src.descriptor_set_layout ) {
    dest[ "descriptor_set_layout" ][ std::to_string( v.first ) ] = *v.second;
  }
  dest[ "external_descriptor_set" ] = nlohmann::json::object();
  for( const auto &v: src.external_descriptor_set ) {
    dest[ "external_descriptor_set" ][ std::to_string( v.first ) ] = *v.second;
  }
  if( src.external_pipeline_layout ) {
    dest[ "external_pipeline_layout" ] = *src.external_pipeline_layout;
  }
  dest[ "resources" ] = src.resources;
  dest[ "swapchain_image_count" ] = src.swapchain_image_count;
  dest[ "ignore_unused_descriptor" ] = src.ignore_unused_descriptor;
  dest[ "specs" ] = nlohmann::json::object();
  for( const auto &v: src.specs ) {
    dest[ "specs" ][ std::to_string( v.first ) ] = v.second;
  }
  dest[ "dim" ] = nlohmann::json::array();
  dest[ "dim" ].push_back( src.dim[ 0 ] );
  dest[ "dim" ].push_back( src.dim[ 1 ] );
  dest[ "dim" ].push_back( src.dim[ 2 ] );
}

}

