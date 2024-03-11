#ifndef GCT_AABB_HPP
#define GCT_AABB_HPP

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gct/setter.hpp>

namespace gct {
struct aabb {
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec3 min;
  glm::vec3 max;
};

using aabb3 = aabb;

struct aabb4 {
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec4 min;
  glm::vec4 max;
};

}

#endif

