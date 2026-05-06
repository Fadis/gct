#ifndef GCT_PROJECT_HPP
#define GCT_PROJECT_HPP

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

namespace gct {

glm::vec2 dimension_reduce( const glm::vec3 &v, const glm::vec3 &plane_normal );

glm::vec3 dimension_expand(
  const glm::vec2 &projected,
  const glm::vec3 &plane_normal
);

}

#endif

