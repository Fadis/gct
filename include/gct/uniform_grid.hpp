#ifndef GCT_UNIFORM_GRID_HPP
#define GCT_UNIFORM_GRID_HPP

#include <cstdint>
#include <vector>
#include <glm/vec2.hpp>

namespace gct {

std::vector< glm::vec2 > generate_uniform_grid(
  std::uint32_t width,
  std::uint32_t height
);
std::vector< glm::vec2 > generate_uniform_grid(
  std::uint32_t point_count,
  const glm::vec2 &scale
);

}

#endif

