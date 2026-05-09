#ifndef GCT_SHADER_GENERATE_NORMAL_H
#define GCT_SHADER_GENERATE_NORMAL_H

#ifdef __cplusplus
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
using namespace glm;
#endif

vec3 generate_normal(
  vec3 p0,
  vec3 p1,
  vec3 p2
) {
  return normalize( cross( p2 - p0, p1 - p0 ) );
}

#endif

