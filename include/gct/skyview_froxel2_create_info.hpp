#ifndef GCT_SKYVIEW_FROXEL2_CREATE_INFO_HPP
#define GCT_SKYVIEW_FROXEL2_CREATE_INFO_HPP

#include <vector>
#include <filesystem>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/allocator_set.hpp>
#include <gct/shader_set.hpp>
#include <gct/matrix_pool.hpp>

namespace gct {

namespace scene_graph {
  class scene_graph_resource;
}

struct skyview_froxel2_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader_set )
  LIBGCT_SHADER_SET( shader_set )
  LIBGCT_SETTER( rename )
  LIBGCT_RENAME_MAP( rename )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( scene_graph )
  LIBGCT_SETTER( node_name )
  LIBGCT_SETTER( froxel_xy_resolution )
  LIBGCT_SETTER( froxel_z_resolution )
  LIBGCT_SETTER( sigma )
  LIBGCT_SETTER( world_to_screen )
  LIBGCT_SETTER( unproject_to_world )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( altitude )
  skyview_froxel2_create_info &set_shader(
    const std::filesystem::path &
  );
  allocator_set_t allocator_set;
  shader_set_t shader_set;
  rename_map_t rename;
  std::vector< named_resource > resources;
  std::shared_ptr< scene_graph::scene_graph_resource > scene_graph;
  std::string node_name;
  std::uint32_t froxel_xy_resolution = 32u;
  std::uint32_t froxel_z_resolution = 32u;
  matrix_pool::matrix_descriptor sigma;
  matrix_pool::matrix_descriptor world_to_screen;
  matrix_pool::matrix_descriptor unproject_to_world;
  float ground_radius = 6360000.f;
  float top_radius = 6460000.f;
  float altitude = 100.f;
  float g = 0.8f;
};

void to_json( nlohmann::json&, const skyview_froxel2_create_info& );

}

#endif


