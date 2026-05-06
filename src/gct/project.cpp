#include <cmath>
#include <glm/geometric.hpp>
#include <gct/project.hpp>

namespace gct {

glm::vec2 dimension_reduce( const glm::vec3 &v, const glm::vec3 &plane_normal ) {
  glm::vec3 t1;
  if( std::abs( plane_normal.y ) > std::abs( plane_normal.z ) ) {
    t1 = glm::vec3( plane_normal.y, -plane_normal.x, 0.0f );
  }
  else {
    t1 = glm::vec3( plane_normal.z, 0.0f, -plane_normal.x );
  }
  t1 = glm::normalize( t1 );
  const glm::vec3 t2 = glm::cross( t1, plane_normal );
  return  glm::vec2(
    glm::dot( v, t1 ),
    glm::dot( v, t2 )
  );
}

glm::vec3 dimension_expand(
  const glm::vec2 &projected,
  const glm::vec3 &plane_normal
) {
  glm::vec3 t1;
  if( std::abs( plane_normal.y ) > std::abs( plane_normal.z ) ) {
    t1 = glm::vec3( plane_normal.y, -plane_normal.x, 0.0f );
  }
  else {
    t1 = glm::vec3( plane_normal.z, 0.0f, -plane_normal.x );
  }
  t1 = glm::normalize( t1 );
  const glm::vec3 t2 = glm::cross( t1, plane_normal );
  return projected.x * t1 + projected.y * t2;
}

}

