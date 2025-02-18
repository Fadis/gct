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
  if( r ) {
    {
      const auto iter = r->descriptor_set_layout.find( r->image_descriptor_set_id );
      if( iter != r->descriptor_set_layout.end() && r->image_descriptor_set ) {
        descriptor_set_layout[ r->image_descriptor_set_id ] = iter->second;
        external_descriptor_set[ r->image_descriptor_set_id ] = r->image_descriptor_set;
      }
    }
    {
      const auto iter = r->descriptor_set_layout.find( r->texture_descriptor_set_id );
      if( iter != r->descriptor_set_layout.end() && r->texture_descriptor_set ) {
        descriptor_set_layout[ r->texture_descriptor_set_id ] = iter->second;
        external_descriptor_set[ r->texture_descriptor_set_id ] = r->texture_descriptor_set;
      }
    }
    if( r->matrix ) {
      add_resource( { "matrix_pool", r->matrix->get_buffer() } );
    }
    if( r->aabb ) {
      add_resource( { "aabb_pool", r->aabb->get_buffer() } );
    }
    if( r->primitive_resource_index ) {
      add_resource( { "primitive_resource_index", r->primitive_resource_index->get_buffer() } );
    }
    if( r->instance_resource_index ) {
      add_resource( { "instance_resource_index", r->instance_resource_index->get_buffer() } );
    }
    if( r->visibility ) {
      add_resource( { "visibility", r->visibility->get_buffer() } );
    }
    if( r->light ) {
      add_resource( { "light_pool", r->light->get_buffer() } );
    }
    ignore_unused_descriptor = true;
  }
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

