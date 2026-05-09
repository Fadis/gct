#ifndef GCT_SHADER_SIGNED_OCTAHEDRON_HPP
#define GCT_SHADER_SIGNED_OCTAHEDRON_HPP

#ifdef __cplusplus
#include <cfloat>
#include <cmath>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
using namespace std;
using namespace glm;
using uint = std::uint32_t;
#else
#include <gct/float.hpp>
#endif

struct signed_octahedron {
  vec2 v;
  uint sign;
  uint reserved;
};

struct signed_diamond {
  float v;
  uint sign;
  uint flip;
  uint reserved;
};

signed_octahedron encode_signed_octahedron( vec3 decoded ) {
  signed_octahedron encoded;
  const vec3 n = decoded / ( abs( decoded.x ) + abs( decoded.y ) + abs( decoded.z ) );
  encoded.v.y = n.y * 0.5f + 0.5f;
  encoded.v.x = n.x * 0.5f + encoded.v.y;
  encoded.v.y = n.x * -0.5f + encoded.v.y;
  encoded.sign = int( clamp( n.z * FLT_MAX, 0.0f, 1.0f ) );
  return encoded;
}

vec3 decode_signed_octahedron( signed_octahedron encoded ) {

  vec3 decoded;
  decoded.x = encoded.v.x - encoded.v.y;
  decoded.y = ( encoded.v.x + encoded.v.y ) - 1.0f;
  decoded.z = encoded.sign * 2.01f - 1.0f;
  decoded.z = decoded.z * ( 1.0f - abs( decoded.x ) - abs( decoded.y ) );
  return normalize( decoded );
}

signed_diamond encode_signed_diamond( vec2 decoded ) {
  float m = abs( decoded.x ) + abs( decoded.y );
  float x = decoded.x / m;
  signed_diamond encoded;
  encoded.sign = ( decoded.y >= 0.0f ) ? 1u : 0u;
  encoded.v = x * 0.5f + 0.5f;
  encoded.flip = 0;
  return encoded;
}

vec2 decode_signed_diamond( signed_diamond encoded ) {
  vec2 decoded;
  const float s = ( encoded.sign != 0u ) ? 1.0f : -1.0f;
  decoded.x = 2.0f * encoded.v - 1.0f;
  decoded.y = s * ( 1.0f - abs( decoded.x ) );
  return normalize( decoded );
}

#endif


