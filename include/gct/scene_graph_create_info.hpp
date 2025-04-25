#ifndef GCT_SCENE_GRAPH_CREATE_INFO_HPP
#define GCT_SCENE_GRAPH_CREATE_INFO_HPP
#include <cstdint>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/matrix_pool_create_info.hpp>
#include <gct/aabb_pool_create_info.hpp>
#include <gct/image_pool_create_info.hpp>
#include <gct/texture_pool_create_info.hpp>
#include <gct/sampler_pool_create_info.hpp>
#include <gct/buffer_pool_create_info.hpp>
#include <gct/light_pool_create_info.hpp>
#include <gct/shader_flag.hpp>
#include <gct/vertex_buffer_pool_create_info.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/pool.hpp>
#include <gct/raw_resource_pair_type.hpp>
#include <gct/allocator_set.hpp>

namespace gct::scene_graph {

struct scene_graph_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( master_shader )
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( image )
  LIBGCT_SETTER( texture )
  LIBGCT_SETTER( sampler )
  LIBGCT_SETTER( primitive_resource_index )
  LIBGCT_SETTER( instance_resource_index )
  LIBGCT_SETTER( visibility )
  LIBGCT_SETTER( accessor )
  LIBGCT_SETTER( mesh )
  LIBGCT_SETTER( lod )
  LIBGCT_SETTER( vertex )
  LIBGCT_SETTER( light )
  LIBGCT_SETTER( prim_pool_size )
  LIBGCT_SETTER( inst_pool_size )
  LIBGCT_SETTER( descriptor_set_id )
  LIBGCT_SETTER( texture_descriptor_set_id )
  LIBGCT_SETTER( image_descriptor_set_id )
  LIBGCT_SETTER( vertex_buffer_descriptor_set_id )
  LIBGCT_SETTER( enable_linear )
  scene_graph_create_info();
  scene_graph_create_info &add_master_shader( const std::filesystem::path &p ) {
    master_shader.push_back( p );
    return *this;
  }
  scene_graph_create_info &set_shader( const std::filesystem::path &p );
  allocator_set_t allocator_set;
  std::vector< std::filesystem::path > master_shader;
  matrix_pool_create_info matrix;
  aabb_pool_create_info aabb;
  image_pool_create_info image;
  texture_pool_create_info texture;
  sampler_pool_create_info sampler;
  buffer_pool_create_info primitive_resource_index;
  buffer_pool_create_info instance_resource_index;
  buffer_pool_create_info visibility;
  buffer_pool_create_info accessor;
  buffer_pool_create_info mesh;
  buffer_pool_create_info lod;
  vertex_buffer_pool_create_info vertex;
  light_pool_create_info light;
  std::uint32_t prim_pool_size = 65536u;
  std::uint32_t inst_pool_size = 65536u;
  std::uint32_t descriptor_set_id = 0u;
  std::uint32_t texture_descriptor_set_id = 2u;
  std::uint32_t image_descriptor_set_id = 3u;
  std::uint32_t vertex_buffer_descriptor_set_id = 4u;
  bool enable_linear = false;
};

void to_json( nlohmann::json&, const scene_graph_create_info& );

}

#endif

