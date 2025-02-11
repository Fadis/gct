#ifndef GCT_SCENE_GRAPH_RESOURCE_HPP
#define GCT_SCENE_GRAPH_RESOURCE_HPP
#include <string>
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/image_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/light_pool.hpp>
#include <gct/shader_flag.hpp>
#include <gct/vertex_buffer_pool.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/pool.hpp>
#include <gct/array_of.hpp>
#include <gct/raw_resource_pair_type.hpp>
#include <gct/color_space.hpp>
#include <gct/scene_graph_create_info.hpp>

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

struct instance_descriptor {
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( resource_index )
  LIBGCT_SETTER( prim )
  LIBGCT_SETTER( visibility )
  matrix_pool::matrix_descriptor matrix;
  aabb_pool::aabb_descriptor aabb;
  buffer_pool::buffer_descriptor resource_index;
  pool< std::shared_ptr< primitive > >::descriptor prim;
  buffer_pool::buffer_descriptor visibility;
};

struct instance {
  LIBGCT_SETTER( initial_world_matrix )
  LIBGCT_SETTER( initial_world_aabb )
  LIBGCT_SETTER( descriptor )
  glm::mat4 initial_world_matrix;
  aabb4 initial_world_aabb;
  instance_descriptor descriptor;
};

void to_json( nlohmann::json &dest, const instance &src );

struct scene_graph_resource {
  scene_graph_resource(
    const scene_graph_create_info &ci
  ) : prim( ci.prim_pool_size ), inst( ci.inst_pool_size ) {}
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( primitive_resource_index )
  LIBGCT_SETTER( instance_resource_index )
  LIBGCT_SETTER( visibility )
  LIBGCT_SETTER( last_visibility )
  LIBGCT_SETTER( resource_pair )
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( light )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( descriptor_set )
  LIBGCT_SETTER( texture_descriptor_set )
  LIBGCT_SETTER( image_descriptor_set )
  LIBGCT_SETTER( descriptor_set_id )
  LIBGCT_SETTER( texture_descriptor_set_id )
  LIBGCT_SETTER( image_descriptor_set_id )
  LIBGCT_SETTER( pipeline_layout )
  LIBGCT_SETTER( push_constant_mp )
  LIBGCT_SETTER( prim )
  LIBGCT_SETTER( inst )
  LIBGCT_SETTER( csmat )
  std::shared_ptr< matrix_pool > matrix;
  std::shared_ptr< aabb_pool > aabb;
  std::shared_ptr< image_pool > image;
  std::shared_ptr< texture_pool > texture;
  std::shared_ptr< sampler_pool > sampler;
  std::shared_ptr< buffer_pool > primitive_resource_index;
  std::shared_ptr< buffer_pool > instance_resource_index;
  std::shared_ptr< buffer_pool > visibility;
  std::shared_ptr< mappable_buffer_t > last_visibility;
  std::shared_ptr< mappable_buffer_t > resource_pair;
  std::shared_ptr< vertex_buffer_pool > vertex;
  std::shared_ptr< light_pool > light;
  std::vector< std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::shared_ptr< descriptor_set_t > descriptor_set;
  std::shared_ptr< descriptor_set_t > image_descriptor_set;
  std::shared_ptr< descriptor_set_t > texture_descriptor_set;
  std::uint32_t descriptor_set_id = 0u;
  std::uint32_t texture_descriptor_set_id = 2u;
  std::uint32_t image_descriptor_set_id = 3u;
  std::shared_ptr< pipeline_layout_t > pipeline_layout;
  std::unordered_map< std::string, uint32_t > attr2index;
  std::optional< spv_member_pointer > push_constant_mp;
  pool< std::shared_ptr< primitive > > prim;
  pool< std::shared_ptr< instance > > inst;
  color_space_matrix csmat;
};

void to_json( nlohmann::json&, const scene_graph_resource& );

}

#endif
