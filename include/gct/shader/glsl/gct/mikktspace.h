#ifndef GCT_SHADER_MIKKTSPACE_H
#define GCT_SHADER_MIKKTSPACE_H

#ifdef __cplusplus
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
using namespace glm;
#endif

vec3 mikktspace(
  vec3 p0,
  vec3 p1,
  vec3 p2,
  vec2 t0,
  vec2 t1,
  vec2 t2
) {
  const vec3 p10 = p1 - p0;
  const vec3 p20 = p2 - p0;
  const vec2 t10 = t1 - t0;
  const vec2 t20 = t2 - t0;

  const float det = 1.0 / ( t10.x * t20.y - t10.y * t20.x );
	const vec3 t = det * ( p10 * t20.y - p20 * t10.y );
  return normalize( t );
}

#endif

