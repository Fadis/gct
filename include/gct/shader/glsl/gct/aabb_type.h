#ifndef GCT_SHADER_AABB_TYPE_H
#define GCT_SHADER_AABB_TYPE_H

#ifdef __cplusplus
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat2x2.hpp>
#include <glm/mat3x3.hpp>
#include <glm/mat4x4.hpp>
using namespace glm;
using uint = std::uint32_t;
#endif

struct aabb2_type {
  vec2 min;
  vec2 max;
};

struct aabb3_type {
  vec3 min;
  vec3 max;
};

struct aabb4_type {
  vec4 min;
  vec4 max;
};
#define aabb_type aabb4_type

bool inside( aabb2_type aabb, vec2 p ) {
  return
    p.x < aabb.max.x &&
    p.x >= aabb.min.x &&
    p.y < aabb.max.y &&
    p.y >= aabb.min.y;
}

bool inside( aabb3_type aabb, vec3 p ) {
  return
    p.x < aabb.max.x &&
    p.x >= aabb.min.x &&
    p.y < aabb.max.y &&
    p.y >= aabb.min.y &&
    p.z < aabb.max.z &&
    p.z >= aabb.min.z;
}

bool inside( aabb4_type aabb, vec4 p ) {
  return
    p.x < aabb.max.x &&
    p.x >= aabb.min.x &&
    p.y < aabb.max.y &&
    p.y >= aabb.min.y &&
    p.z < aabb.max.z &&
    p.z >= aabb.min.z;
}

vec2 get_aabb_vertex( aabb2_type aabb, uint i ) {
  return vec2(
    ( ( i & 2 ) != 0 ) ? aabb.max.x : aabb.min.x,
    ( ( i & 1 ) != 0 ) ? aabb.max.y : aabb.min.y
  );
}

vec3 get_aabb_vertex( aabb3_type aabb, uint i ) {
  return vec3(
    ( ( i & 4 ) != 0 ) ? aabb.max.x : aabb.min.x,
    ( ( i & 2 ) != 0 ) ? aabb.max.y : aabb.min.y,
    ( ( i & 1 ) != 0 ) ? aabb.max.z : aabb.min.z
  );
}

vec4 get_aabb_vertex( aabb4_type aabb, uint i ) {
  return vec4(
    ( ( i & 4 ) != 0 ) ? aabb.max.x : aabb.min.x,
    ( ( i & 2 ) != 0 ) ? aabb.max.y : aabb.min.y,
    ( ( i & 1 ) != 0 ) ? aabb.max.z : aabb.min.z,
    1.0
  );
}

aabb2_type transform_aabb( mat2 m, aabb2_type b ) {
  aabb2_type result;
  vec2 p = m * get_aabb_vertex( b, 0 );
  result.min = p;
  result.max = p;
  for( uint i = 1u; i != 4u; ++i ) {
    p = m * get_aabb_vertex( b, i );
    result.min = min( result.min, p );
    result.max = max( result.max, p );
  }
  return result;
}

aabb3_type transform_aabb( mat3 m, aabb3_type b ) {
  aabb3_type result;
  vec3 p = m * get_aabb_vertex( b, 0 );
  result.min = p;
  result.max = p;
  for( uint i = 1u; i != 8u; ++i ) {
    p = m * get_aabb_vertex( b, i );
    result.min = min( result.min, p );
    result.max = max( result.max, p );
  }
  return result;
}

aabb4_type transform_aabb( mat4 m, aabb4_type b ) {
  aabb4_type result;
  vec4 p = m * get_aabb_vertex( b, 0 );
  p /= p.w;
  result.min = p;
  result.max = p;
  for( uint i = 1u; i != 8u; ++i ) {
    p = m * get_aabb_vertex( b, i );
    p /= p.w;
    result.min = min( result.min, p );
    result.max = max( result.max, p );
  }
  return result;
}

aabb2_type clip_aabb( aabb2_type b ) {
  aabb2_type temp;
  temp.min = max( b.min, vec2( -1, -1 ) );
  temp.max = min( b.max, vec2(  1,  1 ) );
  return temp;
}
aabb3_type clip_aabb( aabb3_type b ) {
  aabb3_type temp;
  temp.min = max( b.min, vec3( -1, -1, -1 ) );
  temp.max = min( b.max, vec3(  1,  1,  1 ) );
  return temp;
}
aabb4_type clip_aabb( aabb4_type b ) {
  aabb4_type temp;
  temp.min = max( b.min, vec4( -1, -1, -1, 1 ) );
  temp.max = min( b.max, vec4(  1,  1,  1, 1 ) );
  return temp;
}

aabb2_type aabb_and( aabb2_type b, aabb2_type c ) {
  aabb2_type temp;
  temp.min = max( b.min, c.min );
  temp.max = min( b.max, c.max );
  temp.min = min( temp.min, temp.max );
  return temp;
}
aabb3_type aabb_and( aabb3_type b, aabb3_type c ) {
  aabb3_type temp;
  temp.min = max( b.min, c.min );
  temp.max = min( b.max, c.max );
  temp.min = min( temp.min, temp.max );
  return temp;
}
aabb4_type aabb_and( aabb4_type b, aabb4_type c ) {
  aabb4_type temp;
  temp.min = max( b.min, c.min );
  temp.max = min( b.max, c.max );
  temp.min = min( temp.min, temp.max );
  return temp;
}

void aabb_append(
#ifdef __cplusplus
  aabb2_type &b,
#else
  inout aabb2_type b,
#endif
  vec2 p
) {
  b.min = min( b.min, p );
  b.max = max( b.max, p );
}

void aabb_append(
#ifdef __cplusplus
  aabb3_type &b,
#else
  inout aabb3_type b,
#endif
  vec3 p
) {
  b.min = min( b.min, p );
  b.max = max( b.max, p );
}

void aabb_append(
#ifdef __cplusplus
  aabb4_type &b,
#else
  inout aabb4_type b,
#endif
  vec4 p
) {
  vec3 b_min = min( vec3( b.min ), vec3( p ) );
  vec3 b_max = max( vec3( b.max ), vec3( p ) );
  b.min = vec4( b_min, b.min.w );
  b.max = vec4( b_max, b.max.w );
}

#endif

