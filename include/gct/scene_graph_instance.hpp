#ifndef GCT_SCENE_GRAPH_INSTANCE_HPP
#define GCT_SCENE_GRAPH_INSTANCE_HPP
#include <memory>
#include <glm/mat4x4.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/pool.hpp>
#include <gct/matrix_pool.hpp>
#include <gct/aabb_pool.hpp>
#include <gct/buffer_pool.hpp>
#include <gct/scene_graph_rigid.hpp>

namespace gct {

class pipeline_layout_t;
}

namespace gct::scene_graph {

class primitive;

struct instance_descriptor {
  LIBGCT_SETTER( matrix )
  LIBGCT_SETTER( aabb )
  LIBGCT_SETTER( resource_index )
  LIBGCT_SETTER( prim )
  LIBGCT_SETTER( visibility )
  LIBGCT_SETTER( meshlet_index )
  LIBGCT_SETTER( rigid )
  matrix_pool::matrix_descriptor matrix;
  aabb_pool::aabb_descriptor aabb;
  buffer_pool::buffer_descriptor resource_index;
  pool< std::shared_ptr< primitive > >::descriptor prim;
  buffer_pool::buffer_descriptor visibility;
  buffer_pool::buffer_descriptor meshlet_index;
  buffer_pool::buffer_descriptor rigid;
};

struct instance {
  LIBGCT_SETTER( initial_world_matrix )
  LIBGCT_SETTER( initial_world_aabb )
  LIBGCT_SETTER( descriptor )
  LIBGCT_SETTER( is_highest_lod )
  LIBGCT_SETTER( mesh_task_offset )
  LIBGCT_SETTER( mesh_task_count )
  LIBGCT_SETTER( lod_id )
  LIBGCT_SETTER( rigid_state )
  glm::mat4 initial_world_matrix;
  aabb4 initial_world_aabb;
  instance_descriptor descriptor;
  bool is_highest_lod = true;
  std::uint32_t mesh_task_offset = 0u;
  std::uint32_t mesh_task_count = 0u;
  std::uint32_t lod_id = 0u;
  std::optional< rigid > rigid_state;
};

void to_json( nlohmann::json &dest, const instance &src );

}

#endif
