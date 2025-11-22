#ifndef GCT_SCENE_GRAPH_RIGID_HPP
#define GCT_SCENE_GRAPH_RIGID_HPP
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

struct rigid_descriptor {
  LIBGCT_SETTER( trs )
  LIBGCT_SETTER( trs_previous )
  LIBGCT_SETTER( collision_constraint )
  matrix_pool::matrix_descriptor trs;
  matrix_pool::matrix_descriptor trs_previous;
  matrix_pool::matrix_descriptor trs_previous_coarse;
  buffer_pool::buffer_descriptor collision_constraint;
};

void to_json( nlohmann::json &dest, const rigid_descriptor &src );

struct rigid {
  rigid_descriptor descriptor;
};

void to_json( nlohmann::json &dest, const rigid &src );

}

#endif

