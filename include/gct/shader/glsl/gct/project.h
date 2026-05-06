#ifndef GCT_SHADER_PROJECT_H
#define GCT_SHADER_PROJECT_H

#ifdef __cplusplus
#include <cfloat>
#include <cmath>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
using namespace std;
using namespace glm;
#endif

vec2 dimension_reduce( vec3 v, vec3 plane_normal ) {
  vec3 t1;
  if( abs( plane_normal.y ) > abs( plane_normal.z ) ) {
    t1 = vec3( plane_normal.y, -plane_normal.x, 0.0f );
  }
  else {
    t1 = vec3( plane_normal.z, 0.0f, -plane_normal.x );
  }
  t1 = normalize( t1 );
  const vec3 t2 = cross( t1, plane_normal );
  return  vec2(
    dot( v, t1 ),
    dot( v, t2 )
  );
}

vec3 dimension_expand(
  vec2 projected,
  vec3 plane_normal
) {
  vec3 t1;
  if( abs( plane_normal.y ) > abs( plane_normal.z ) ) {
    t1 = vec3( plane_normal.y, -plane_normal.x, 0.0f );
  }
  else {
    t1 = vec3( plane_normal.z, 0.0f, -plane_normal.x );
  }
  t1 = normalize( t1 );
  const vec3 t2 = cross( t1, plane_normal );
  return projected.x * t1 + projected.y * t2;
}

vec2 project( vec2 v, vec2 n  ) {
  return v - dot( v, n ) * n;
}

vec3 project( vec3 v, vec3 n  ) {
  return v - dot( v, n ) * n;
}

#endif

