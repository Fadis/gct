#ifndef GCT_SIMILAR_MATRIX_HPP
#define GCT_SIMILAR_MATRIX_HPP

#include <glm/mat4x4.hpp>
namespace gct {

[[nodiscard]] bool similar_matrix(
  const glm::mat4 &a,
  const glm::mat4 &b
);

}

#endif

