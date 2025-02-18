#ifndef GCT_SCENE_GRAPH_PRIMITIVE_HPP
#define GCT_SCENE_GRAPH_PRIMITIVE_HPP
#include <string>
#include <cstdint>
#include <memory>
#include <vector>
#include <unordered_map>
#include <glm/vec4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/texture_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/vertex_buffer_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/shader_flag.hpp>
#include <gct/graphics_pipeline_create_info.hpp>

namespace gct {

class pipeline_layout_t;
}

namespace gct::scene_graph {

struct buffer_offset {
  LIBGCT_SETTER( buffer )
  LIBGCT_SETTER( offset )
  vertex_buffer_pool::vertex_buffer_descriptor buffer;
  std::uint32_t offset = 0u;
};
void to_json( nlohmann::json&, const buffer_offset& );

struct primitive_descriptor {
  LIBGCT_SETTER( base_color_texture )
  LIBGCT_SETTER( metallic_roughness_texture )
  LIBGCT_SETTER( normal_texture )
  LIBGCT_SETTER( occlusion_texture )
  LIBGCT_SETTER( emissive_texture )
  LIBGCT_SETTER( resource_index )
  LIBGCT_SETTER( aabb )
  texture_pool::texture_descriptor base_color_texture;
  texture_pool::texture_descriptor metallic_roughness_texture;
  texture_pool::texture_descriptor normal_texture;
  texture_pool::texture_descriptor occlusion_texture;
  texture_pool::texture_descriptor emissive_texture;
  buffer_pool::buffer_descriptor resource_index;
  aabb_pool::aabb_descriptor aabb;
};

void to_json( nlohmann::json &dest, const primitive_descriptor &src );

struct primitive {
  LIBGCT_SETTER( vertex_buffer )
  LIBGCT_SETTER( indexed )
  LIBGCT_SETTER( index_buffer )
  LIBGCT_SETTER( count )
  LIBGCT_SETTER( index_buffer_type )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( vertex_input_binding )
  LIBGCT_SETTER( vertex_input_attribute )
  LIBGCT_SETTER( vs_flag )
  LIBGCT_SETTER( gs_flag )
  LIBGCT_SETTER( fs_flag )
  LIBGCT_SETTER( cull )
  LIBGCT_SETTER( blend )
  LIBGCT_SETTER( roughness )
  LIBGCT_SETTER( metalness )
  LIBGCT_SETTER( emissive )
  LIBGCT_SETTER( base_color )
  LIBGCT_SETTER( normal_scale )
  LIBGCT_SETTER( occlusion_strength )
  LIBGCT_SETTER( pipeline_create_info )
  LIBGCT_SETTER( descriptor )
  std::unordered_map< std::uint32_t, buffer_offset > vertex_buffer;
  bool indexed = false;
  buffer_offset index_buffer;
  uint32_t count = 0u;
  vk::IndexType index_buffer_type;
  aabb4 aabb;
  std::vector< vk::VertexInputBindingDescription > vertex_input_binding;
  std::vector< vk::VertexInputAttributeDescription > vertex_input_attribute;
  shader_flag_t vs_flag;
  shader_flag_t gs_flag;
  shader_flag_t fs_flag;
  bool cull = false;
  bool blend = false;
  float roughness = 0.f;
  float metalness = 0.f;
  glm::vec4 emissive = glm::vec4( 0.f, 0.f, 0.f, 1.f );
  glm::vec4 base_color = glm::vec4( 1.f, 1.f, 1.f, 1.f );
  float normal_scale = 1.f;
  float occlusion_strength = 0.f;
  graphics_pipeline_create_info_t pipeline_create_info;
  primitive_descriptor descriptor;
};

void to_json( nlohmann::json &dest, const primitive &src );

}

#endif
