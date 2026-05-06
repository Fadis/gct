#ifndef GCT_SHADER_SIMEQ_H
#define GCT_SHADER_SIMEQ_H

#ifdef __cplusplus
#include <cfloat>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
using namespace std;
using namespace glm;
#endif


bool simeq( const float a, const float b, const float threshold ) {
  return
    ( abs( b - a ) ) < threshold;
}

bool simeq( const vec3 a, const vec2 b, const float threshold ) {
  return
    ( abs( b.x - a.x ) + abs( b.y -a.y ) ) < threshold;
}

bool simeq( const vec3 a, const vec3 b, const float threshold ) {
  return
    ( abs( b.x - a.x ) + abs( b.y -a.y ) + abs( b.z - a.z ) ) < threshold;
}

bool simeq( const vec4 a, const vec4 b, const float threshold ) {
  return
    ( abs( b.x - a.x ) + abs( b.y -a.y ) + abs( b.z - a.z ) + abs( b.w - a.w ) ) < threshold;
}

#endif

