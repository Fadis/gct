#ifndef GCT_GAUSSIAN_BLUE_NOISE_HPP
#define GCT_GAUSSIAN_BLUE_NOISE_HPP

#include <vector>
#include <glm/vec2.hpp>

namespace gct {

std::vector< glm::vec2 > gaussian_blue_noise(
  const std::vector< float > &dist,
  int width,
  int point_count,
  int iterations
);

std::vector< glm::vec2 > gaussian_blue_noise(
  int point_count,
  int iterations
);

}

#endif
