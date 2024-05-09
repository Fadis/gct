#ifndef GCT_PROJECTION_MATRIX_HPP
#define GCT_PROJECTION_MATRIX_HPP
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <gct/aabb.hpp>

namespace gct {
float split_depth(
  float near,
  float far,
  std::uint32_t i,
  std::uint32_t m
);

glm::mat4 split_projection_matrix(
  const glm::mat4 &projection,
  std::uint32_t i,
  std::uint32_t m
);

aabb get_clipping_aabb(
  const glm::mat4 &matrix
);

std::vector< aabb > get_clipping_aabb(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix,
  std::uint32_t m
);

std::vector< std::pair< glm::vec4, glm::vec4 > > get_clippling_area_edges(
  const glm::mat4 &projection_matrix,
  const glm::mat4 &camera_matrix
);

bool is_visible(
  const glm::mat4 &matrix,
  const aabb &box
);

bool is_visible(
  const glm::mat4 &matrix,
  const aabb4 &box
);

}

#endif
