#include <gct/mikktspace.hpp>
#include <glm/geometric.hpp>

namespace gct {

glm::vec3 mikktspace(
  const glm::vec3 &p0,
  const glm::vec3 &p1,
  const glm::vec3 &p2,
  const glm::vec2 &t0,
  const glm::vec2 &t1,
  const glm::vec2 &t2
) {
  const glm::vec3 p10 = p1 - p0;
  const glm::vec3 p20 = p2 - p0;
  const glm::vec2 t10 = t1 - t0;
  const glm::vec2 t20 = t2 - t0;

  const float det = 1.0 / ( t10.x * t20.y - t10.y * t20.x );
	const glm::vec3 t = det * ( p10 * t20.y - p20 * t10.y );
  return glm::normalize( t );
}

}

