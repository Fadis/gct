#include <nlohmann/json.hpp>
#include <vulkan2json/IndexType.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/scene_graph_primitive.hpp>
#include <gct/scene_graph_instance.hpp>
#include <gct/scene_graph_create_info.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/image_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/light_pool.hpp>
#include <gct/vertex_buffer_pool.hpp>
#include <gct/pool.hpp>
#include <gct/scene_graph_resource.hpp>

namespace gct::scene_graph {
  
scene_graph_resource::scene_graph_resource(
  const scene_graph_create_info &ci
) : prim( ci.prim_pool_size ), inst( ci.inst_pool_size ) {}

scene_graph_resource::operator std::vector< named_resource > () {
  std::vector< named_resource > n;
  if( primitive_resource_index ) {
    n.emplace_back( "primitive_resource_index", primitive_resource_index->get_buffer() );
  }
  if( instance_resource_index ) {
    n.emplace_back( "instance_resource_index", instance_resource_index->get_buffer() );
  }
  if( visibility ) {
    n.emplace_back( "visibility_pool", visibility->get_buffer() );
  }
  if( matrix ) {
    n.emplace_back( "matrix_pool", matrix->get_buffer() );
  }
  if( aabb ) {
    n.emplace_back( "aabb_pool", aabb->get_buffer() );
  }
  /*if( texture ) {
    n.emplace_back( "texture_pool", sampler-> );
  }
  if( image ) {
    n.emplace_back( "image_pool8", visibility->get_buffer() );
  }*/
  if( light ) {
    n.emplace_back( "light_pool", light->get_buffer() );
  }
  return n;
}


void to_json( nlohmann::json &dest, const scene_graph_resource &src ) {
  dest = nlohmann::json::object();
  if( src.matrix ) {
    dest[ "matrix" ] = *src.matrix;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
  if( src.image ) {
    dest[ "image" ] = *src.image;
  }
  if( src.texture ) {
    dest[ "texture" ] = *src.texture;
  }
  if( src.sampler ) {
    dest[ "sampler" ] = *src.sampler;
  }
  if( src.primitive_resource_index ) {
    dest[ "primitive_resource_index" ] = *src.primitive_resource_index;
  }
  if( src.visibility ) {
    dest[ "visibility" ] = *src.visibility;
  }
  if( src.vertex ) {
    dest[ "vertex" ] = *src.vertex;
  }
  if( src.light ) {
    dest[ "light" ] = *src.light;
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
  if( src.descriptor_set ) {
    dest[ "descriptor_set" ] = *src.descriptor_set;
  }
  if( src.texture_descriptor_set ) {
    dest[ "texture_descriptor_set" ] = *src.texture_descriptor_set;
  }
  dest[ "primitive" ] = nlohmann::json::array();
  for( const auto &v: src.prim.get_value() ) {
    dest[ "primitive" ].push_back( *v );
  }
  dest[ "instance" ] = nlohmann::json::array();
  for( const auto &v: src.inst.get_value() ) {
    dest[ "instance" ].push_back( *v );
  }
}

}

