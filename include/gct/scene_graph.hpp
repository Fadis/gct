#ifndef GCT_SCENE_GRAPH_HPP
#define GCT_SCENE_GRAPH_HPP
#include <iostream>
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

struct scene_graph_create_info {
  LIBGCT_SETTER( allocator )
  LIBGCT_SETTER( descriptor_pool )
  LIBGCT_SETTER( pipeline_cache )
  LIBGCT_SETTER( master_shader )
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( primitive_resource_index )
  LIBGCT_SETTER( instance_resource_index )
  LIBGCT_SETTER( visibility )
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( light )
  LIBGCT_SETTER( prim_pool_size )
  LIBGCT_SETTER( inst_pool_size )
  LIBGCT_SETTER( descriptor_set_id )
  LIBGCT_SETTER( texture_descriptor_set_id )
  LIBGCT_SETTER( image_descriptor_set_id )
  LIBGCT_SETTER( enable_linear )
  scene_graph_create_info();
  scene_graph_create_info &add_master_shader( const std::filesystem::path &p ) {
    master_shader.push_back( p );
    return *this;
  }
  scene_graph_create_info &set_shader( const std::filesystem::path &p );
  std::shared_ptr< allocator_t > allocator;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::vector< std::filesystem::path > master_shader;
  matrix_pool_create_info matrix;
  aabb_pool_create_info aabb;
  image_pool_create_info image;
  texture_pool_create_info texture;
  sampler_pool_create_info sampler;
  buffer_pool_create_info primitive_resource_index;
  buffer_pool_create_info instance_resource_index;
  buffer_pool_create_info visibility;
  vertex_buffer_pool_create_info vertex;
  light_pool_create_info light;
  std::uint32_t prim_pool_size = 65536u;
  std::uint32_t inst_pool_size = 65536u;
  std::uint32_t descriptor_set_id = 0u;
  std::uint32_t texture_descriptor_set_id = 2u;
  std::uint32_t image_descriptor_set_id = 3u;
  bool enable_linear = false;
};

void to_json( nlohmann::json&, const scene_graph_create_info& );

struct primitive;
struct instance;

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

void to_json( nlohmann::json &dest, const instance &src );

struct instance {
  LIBGCT_SETTER( initial_world_matrix )
  LIBGCT_SETTER( initial_world_aabb )
  LIBGCT_SETTER( descriptor )
  glm::mat4 initial_world_matrix;
  aabb4 initial_world_aabb;
  instance_descriptor descriptor;
};

struct node {
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const std::shared_ptr< scene_graph_resource > &res,
    const glm::mat4 &local_matrix
  ) : props( ci ), resource( res ) {
    matrix = resource->matrix->allocate( local_matrix );
    initial_world_matrix = local_matrix;
  }
  node(
    const std::shared_ptr< scene_graph_create_info > &ci,
    const std::shared_ptr< scene_graph_resource > &res,
    const matrix_pool::matrix_descriptor &parent,
    const glm::mat4 &parent_matrix,
    const glm::mat4 &local_matrix
  ) : props( ci ), resource( res ) {
    matrix = resource->matrix->allocate( parent, local_matrix );
    initial_world_matrix = parent_matrix * local_matrix;
  }
  std::shared_ptr< node > add_child( const glm::mat4 &local_matrix ) {
    child.push_back( std::make_shared< node >(
      props,
      resource,
      matrix,
      initial_world_matrix,
      local_matrix
    ) );
    return child.back();
  }
  void update_matrix( const glm::mat4 &m ) {
    resource->matrix->set( matrix, m );
    for( const auto &v: child ) {
      v->touch();
    }
  }
  void touch() {
    for( auto &v: child ) {
      v->touch();
    }
    for( auto &v: inst ) {
      const auto &i = resource->inst.get( v );
      resource->aabb->touch( i->descriptor.aabb );
    }
  }
  [[nodiscard]] std::optional< aabb4 > get_initial_world_aabb() const;
  void to_json( nlohmann::json& ) const;
  std::string name;
  std::vector< std::shared_ptr< node > > child;
  std::vector< pool< std::shared_ptr< instance > >::descriptor > inst;
  std::vector< light_pool::light_descriptor > light;
  glm::mat4 initial_world_matrix;
  aabb4 prim_aabb;
  matrix_pool::matrix_descriptor matrix;
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< scene_graph_resource > resource;
};

void to_json( nlohmann::json&, const node& );

class scene_graph {
public:
  scene_graph(
    const scene_graph_create_info &ci
  );
  [[nodiscard]] const scene_graph_create_info &get_props() const {
    return *props;
  }
  [[nodiscard]] std::shared_ptr< node > get_root_node() const {
    return root_node;
  }
  [[nodiscard]] const std::shared_ptr< vertex_buffer_pool > &get_vertex() const {
    return resource->vertex;
  }
  [[nodiscard]] const std::shared_ptr< image_pool > &get_image() const {
    return resource->image;
  }
  [[nodiscard]] const std::shared_ptr< texture_pool > &get_texture() const {
    return resource->texture;
  }
  [[nodiscard]] const std::shared_ptr< sampler_pool > &get_sampler() const {
    return resource->sampler;
  }
  [[nodiscard]] const std::shared_ptr< scene_graph_resource > &get_resource() const {
    return resource;
  }
  void to_json( nlohmann::json& ) const;
  void operator()( command_buffer_recorder_t& ) const;
  void rotate_visibility( command_buffer_recorder_t &rec ) const;
  void clear_visibility( command_buffer_recorder_t &rec ) const;
  void fill_visibility( command_buffer_recorder_t &rec, std::uint32_t ) const;
private:
  std::shared_ptr< scene_graph_create_info > props;
  std::shared_ptr< node > root_node;
  std::shared_ptr< scene_graph_resource > resource;
  mutable bool init_visibility = true;
  bool use_conditional = false;
};

void to_json( nlohmann::json &, const scene_graph& );

}

#endif

