#include <nlohmann/json.hpp>
#include <vulkan2json/IndexType.hpp>
#include <vulkan2json/VertexInputBindingDescription.hpp>
#include <vulkan2json/VertexInputAttributeDescription.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/primitive.hpp>

namespace gct::scene_graph {
void to_json( nlohmann::json &dest, const buffer_offset &src ) {
  dest = nlohmann::json::object();
  if( src.buffer ) {
    dest[ "buffer" ] = *src.buffer;
  }
  dest[ "offset" ] = src.offset;
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
  dest[ "descriptor_set_layout" ] = nlohmann::json::array();
  for( const auto &v: src.descriptor_set_layout ) {
    if( v ) {
      dest[ "descriptor_set_layout" ].push_back( *v );
    }
    else {
      dest[ "descriptor_set_layout" ].push_back( nullptr );
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

void to_json( nlohmann::json &dest, const instance_descriptor &src ) {
  dest = nlohmann::json::object();
  if( src.matrix ) {
    dest[ "matrix" ] = *src.matrix;
  }
  if( src.aabb ) {
    dest[ "aabb" ] = *src.aabb;
  }
  if( src.resource_index ) {
    dest[ "resource_index" ] = *src.resource_index;
  }
  if( src.prim ) {
    dest[ "primitive" ] = *src.prim;
  }
  if( src.visibility ) {
    dest[ "visibility" ] = *src.visibility;
  }
};

void to_json( nlohmann::json &dest, const instance &src ) {
  dest = nlohmann::json::object();
  dest[ "descriptor" ] = src.descriptor;
}

}

