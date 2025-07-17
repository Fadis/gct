#include <nlohmann/json.hpp>
#include <vulkan2json/IndexType.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#include <gct/scene_graph_primitive.hpp>

namespace gct::scene_graph {
void to_json( nlohmann::json &dest, const buffer_offset &src ) {
  dest = nlohmann::json::object();
  if( src.buffer ) {
    dest[ "buffer" ] = *src.buffer;
  }
  dest[ "offset" ] = src.offset;
}

void to_json( nlohmann::json &dest, const primitive_descriptor &src ) {
  dest = nlohmann::json::object();
  /*dest[ "vertex_buffer" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_buffer ) {
    dest[ "vertex_buffer" ].push_back( *v );
  }
  dest[ "vertex_buffer_offset" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_buffer_offset ) {
    dest[ "vertex_buffer_offset" ].push_back( v );
  }
  if( src.index_buffer ) {
    dest[ "index_buffer" ] = *src.index_buffer;
  }
  dest[ "index_buffer_offset" ] = src.index_buffer_offset;*/
  if( src.base_color_texture ) {
    dest[ "base_color_texture" ] = *src.base_color_texture;
  }
  if( src.metallic_roughness_texture ) {
    dest[ "metallic_roughness_texture" ] = *src.metallic_roughness_texture;
  }
  if( src.normal_texture ) {
    dest[ "normal_texture" ] = *src.normal_texture;
  }
  if( src.occlusion_texture ) {
    dest[ "occlusion_texture" ] = *src.occlusion_texture;
  }
  if( src.emissive_texture ) {
    dest[ "emissive_texture" ] = *src.emissive_texture;
  }
  if( src.resource_index ) {
    dest[ "resource_index" ] = *src.resource_index;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
  dest[ "accessor" ] = nlohmann::json::array();
  for( const auto &a: src.accessor ) {
    if( a ) {
      dest[ "accessor" ].push_back( *a );
    }
  }
  if( src.mesh ) {
    dest[ "mesh" ] = *src.mesh;
  }
  if( src.meshlet ) {
    dest[ "meshlet" ] = *src.meshlet;
  }
  if( src.particle ) {
    dest[ "particle" ] = *src.particle;
  }
  if( src.distance_constraint ) {
    dest[ "distance_constraint" ] = *src.distance_constraint;
  }
  if( src.constraint ) {
    dest[ "constraint" ] = *src.constraint;
  }
  if( src.fluid_constraint ) {
    dest[ "fluid_constraint" ] = *src.fluid_constraint;
  }
  if( src.rigid_constraint ) {
    dest[ "rigid_constraint" ] = *src.rigid_constraint;
  }
  if( src.vertex_to_primitive ) {
    dest[ "vertex_to_primitive" ] = *src.vertex_to_primitive;
  }
  if( src.lambda ) {
    dest[ "lambda" ] = *src.lambda;
  }
}

void to_json( nlohmann::json &dest, const primitive &src ) {
  dest = nlohmann::json::object();
  dest[ "vertex_buffer" ] = nlohmann::json::object();
  for( const auto &v: src.vertex_buffer ) {
    dest[ "vertex_buffer" ][ std::to_string( v.first ) ] = v.second;
  }
  dest[ "indexed" ] = src.indexed;
  dest[ "index_buffer" ] = src.index_buffer;
  dest[ "count" ] = src.count;
  dest[ "index_buffer_type" ] = src.index_buffer_type;
  dest[ "aabb" ] = src.aabb;
  dest[ "vertex_input_binding" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_input_binding ) {
    dest[ "vertex_input_binding" ].push_back( v );
  }
  dest[ "vertex_input_attribute" ] = nlohmann::json::array();
  for( const auto &v: src.vertex_input_attribute ) {
    dest[ "vertex_input_attribute" ].push_back( v );
  }
  dest[ "vs_flag" ] = int( src.vs_flag );
  dest[ "gs_flag" ] = int( src.gs_flag );
  dest[ "fs_flag" ] = int( src.fs_flag );
  dest[ "ts_flag" ] = int( src.ts_flag );
  dest[ "ms_flag" ] = int( src.ms_flag );
  dest[ "cull" ] = src.cull;
  dest[ "blend" ] = src.blend;
  dest[ "roughness" ] = src.roughness;
  dest[ "emissive" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != 4u; ++i ) {
    dest[ "emissive" ].push_back( src.emissive[ i ] );
  }
  dest[ "base_color" ] = nlohmann::json::array();
  for( unsigned int i = 0u; i != 4u; ++i ) {
    dest[ "base_color" ].push_back( src.emissive[ i ] );
  }
  dest[ "normal_scale" ] = src.normal_scale;
  dest[ "occlusion_strength" ] = src.occlusion_strength;
  dest[ "pipeline_create_info" ] = src.pipeline_create_info;
  dest[ "descriptor" ] = src.descriptor;
}

}

