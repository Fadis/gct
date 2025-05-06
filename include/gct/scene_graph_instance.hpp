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
  matrix_pool::matrix_descriptor matrix;
  aabb_pool::aabb_descriptor aabb;
  buffer_pool::buffer_descriptor resource_index;
  pool< std::shared_ptr< primitive > >::descriptor prim;
  buffer_pool::buffer_descriptor visibility;
  buffer_pool::buffer_descriptor meshlet_index;
};

struct instance {
  LIBGCT_SETTER( initial_world_matrix )
  LIBGCT_SETTER( initial_world_aabb )
  LIBGCT_SETTER( descriptor )
  LIBGCT_SETTER( is_highest_lod )
  glm::mat4 initial_world_matrix;
  aabb4 initial_world_aabb;
  instance_descriptor descriptor;
  bool is_highest_lod = true;
};

void to_json( nlohmann::json &dest, const instance &src );

}

#endif
