#ifndef GCT_SHADER_N21T11_H
#define GCT_SHADER_N21T11_H

#ifdef __cplusplus
#include <cfloat>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>
#include <gct/shader/glsl/gct/project.h>
#include <gct/shader/glsl/gct/signed_octahedron.h>
using namespace std;
using namespace glm;
using uint = std::uint32_t;
#else
#include <gct/project.h>
#include <gct/signed_octahedron.h>
#endif

uint n21t11_pack( signed_octahedron normal ) {
  return
    ( normal.sign << 31 ) |
    ( uint( normal.v.x * 0x3FF ) << 20 ) |
    ( uint( normal.v.y * 0x3FF ) << 10 );
}

uint n21t11_pack( signed_octahedron normal, signed_diamond tangent ) {
  return
    ( normal.sign << 31 ) |
    ( tangent.sign << 30 ) |
    ( uint( normal.v.x * 0x3FF ) << 20 ) |
    ( uint( normal.v.y * 0x3FF ) << 10 ) |
    ( uint( tangent.v * 0x3FF ) );
}

signed_octahedron n21t11_unpack_normal( uint packed ) {
  return signed_octahedron(
    vec2(
      float( ( packed >> 20 ) & 0x3FF ) / 0x3FF,
      float( ( packed >> 10 ) & 0x3FF ) / 0x3FF
    ),
    ( packed >> 31 ) & 0x1u,
    0u
  );
}

signed_diamond n21t11_unpack_tangent( uint packed ) {
  return signed_diamond(
    float( packed & 0x3FF ) / 0x3FF,
    ( packed >> 30 ) & 0x1u,
    0u,
    0u
  );
}

uint n21t11_encode( vec3 normal, vec3 tangent ) {
  const signed_octahedron encoded_normal = encode_signed_octahedron( normal );
  const uint early_packed = n21t11_pack( encoded_normal );
  const signed_octahedron early_unpacked_normal = n21t11_unpack_normal( early_packed );
  const vec3 aligned_normal = decode_signed_octahedron( early_unpacked_normal );
  const vec2 projected_tangent = dimension_reduce( tangent, aligned_normal );
  const signed_diamond encoded_tangent = encode_signed_diamond( projected_tangent );
  const uint packed = n21t11_pack( encoded_normal, encoded_tangent );
  return packed;
}

vec3 n21t11_decode_normal( uint packed ) {
  const signed_octahedron encoded_normal = n21t11_unpack_normal( packed );
  return decode_signed_octahedron( encoded_normal );
}

vec3 n21t11_decode_tangent( uint packed, vec3 normal ) {
  const signed_diamond encoded_tangent = n21t11_unpack_tangent( packed );
  const vec2 projected_tangent = decode_signed_diamond( encoded_tangent );
  return dimension_expand( projected_tangent, normal );
}

#endif

