#ifndef GCT_GENERATE_PROJECTION_MATRIX_HPP
#define GCT_GENERATE_PROJECTION_MATRIX_HPP

#include <cstdint>
#include <vector>
#include <tuple>
#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>
#include <gct/aabb.hpp>

namespace gct {

[[nodiscard]] std::tuple< float, float > generate_cube_projection_distance(
  const glm::vec3 &camera_pos,
  const std::vector< aabb > &aabbs
);

[[nodiscard]] std::tuple< glm::mat4, float, float > generate_projection_matrix(
  const glm::mat4 &camera_matrix,
  float fovy,
  std::uint32_t width,
  std::uint32_t height,
  const std::vector< aabb > &aabbs
);

}

#endif

