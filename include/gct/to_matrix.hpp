#ifndef LIBGCT_INCLUDE_GCT_TO_MATRIX_HPP
#define LIBGCT_INCLUDE_GCT_TO_MATRIX_HPP

#include <array>
#include <glm/mat4x4.hpp>
namespace gct {
  glm::tmat4x4< float > to_matrix(
    const std::array< float, 3 > &t,
    const std::array< float, 4 > &r,
    const std::array< float, 3 > &s
  );
  glm::tmat4x4< float > to_matrix(
    const std::array< float, 16 > &t
  );
}

#endif

