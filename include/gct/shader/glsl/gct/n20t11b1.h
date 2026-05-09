#ifndef GCT_SHADER_N20T11B1_H
#define GCT_SHADER_N20T11B1_H

#ifdef __cplusplus
#include <cfloat>
#include <cstdint>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
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

uint n20t11b1_pack( signed_octahedron normal ) {
  return
    ( normal.sign << 31 ) |
    ( uint( normal.v.x * 0x3FF ) << 20 ) |
    ( uint( normal.v.y * 0x1FF ) << 11 );
}

uint n20t11b1_pack( signed_octahedron normal, signed_diamond tangent ) {
  return
    ( normal.sign << 31 ) |
    ( tangent.sign << 30 ) |
    ( uint( normal.v.x * 0x3FF ) << 20 ) |
    ( uint( normal.v.y * 0x1FF ) << 11 ) |
    ( tangent.flip << 10 ) |
    ( uint( tangent.v * 0x3FF ) );
}

signed_octahedron n20t11b1_unpack_normal( uint packed ) {
  return signed_octahedron(
    vec2(
      float( ( packed >> 20 ) & 0x3FF ) / 0x3FF,
      float( ( packed >> 11 ) & 0x1FF ) / 0x1FF
    ),
    ( packed >> 31 ) & 0x1u,
    0u
  );
}

signed_diamond n20t11b1_unpack_tangent( uint packed ) {
  return signed_diamond(
    float( packed & 0x3FF ) / 0x3FF,
    ( packed >> 30 ) & 0x1u,
    ( packed >> 10 ) & 0x1u,
    0u
  );
}

uint n20t11b1_encode( vec3 normal, vec4 tangent ) {
  const signed_octahedron encoded_normal = encode_signed_octahedron( normal );
  const uint early_packed = n20t11b1_pack( encoded_normal );
  const signed_octahedron early_unpacked_normal = n20t11b1_unpack_normal( early_packed );
  const vec3 aligned_normal = decode_signed_octahedron( early_unpacked_normal );
  const vec2 projected_tangent = dimension_reduce( vec3( tangent ), aligned_normal );
  signed_diamond encoded_tangent = encode_signed_diamond( projected_tangent );
  encoded_tangent.flip = tangent.w < 0.0 ? 1 : 0;
  const uint packed = n20t11b1_pack( encoded_normal, encoded_tangent );
  return packed;
}

vec3 n20t11b1_decode_normal( uint packed ) {
  const signed_octahedron encoded_normal = n20t11b1_unpack_normal( packed );
  return decode_signed_octahedron( encoded_normal );
}

vec4 n20t11b1_decode_tangent( uint packed, vec3 normal ) {
  const signed_diamond encoded_tangent = n20t11b1_unpack_tangent( packed );
  const vec2 projected_tangent = decode_signed_diamond( encoded_tangent );
  const vec3 decoded = dimension_expand( projected_tangent, normal );
  return vec4( decoded, ( encoded_tangent.flip != 0 ) ? -1.0 : 1.0 );
}

#endif


