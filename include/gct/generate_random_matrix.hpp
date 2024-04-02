#ifndef GCT_GENERATE_RANDOM_MATRIX_HPP
#define GCT_GENERATE_RANDOM_MATRIX_HPP

#include <random>
#include <glm/mat4x4.hpp>

namespace gct {

glm::mat4 generate_random_matrix(
  std::uniform_real_distribution<float> &dist,
  std::mt19937 &engine
);

}

#endif

