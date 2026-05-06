#ifndef GCT_MIKKTSPACE_HPP
#define GCT_MIKKTSPACE_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace gct {

glm::vec3 mikktspace(
  const glm::vec3 &p0,
  const glm::vec3 &p1,
  const glm::vec3 &p2,
  const glm::vec2 &t0,
  const glm::vec2 &t1,
  const glm::vec2 &t2
);

}

#endif

