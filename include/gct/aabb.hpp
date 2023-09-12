#ifndef GCT_AABB_HPP
#define GCT_AABB_HPP

#include <glm/vec3.hpp>
#include <gct/setter.hpp>

namespace gct {
struct aabb {
  LIBGCT_SETTER( min )
  LIBGCT_SETTER( max )
  glm::vec3 min;
  glm::vec3 max;
};

}

#endif

