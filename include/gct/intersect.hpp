#ifndef GCT_INTERSECT_HPP
#define GCT_INTERSECT_HPP

#include <array>
#include <glm/vec2.hpp>

namespace gct {

bool intersect(
  const std::array< glm::vec2, 2u > &a,
  const std::array< glm::vec2, 2u > &b,
  bool edge
);

}

#endif

