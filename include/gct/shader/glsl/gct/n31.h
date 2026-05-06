#ifndef GCT_SHADER_N31_H
#define GCT_SHADER_N31_H

#ifdef __cplusplus
#include <cfloat>
#include <cstdint>
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

uint n31_pack( signed_octahedron normal ) {
  return
    ( normal.sign << 30 ) |
    ( uint( normal.v.x * 0x7FFF ) << 15 ) |
    ( uint( normal.v.y * 0x7FFF ) );
}

signed_octahedron n31_unpack( uint packed ) {
  return signed_octahedron{
    vec2(
      float( ( packed >> 15 ) & 0x7FFF ) / 0x7FFF,
      float( ( packed ) & 0x7FFF ) / 0x7FFF
    ),
    ( packed >> 30 ) & 0x1u
  };
}

uint n31_encode( vec3 normal ) {
  const auto encoded_normal = encode_signed_octahedron( normal );
  const auto packed = n31_pack( encoded_normal );
  return packed;
}

vec3 n31_decode( uint packed ) {
  const auto encoded_normal = n31_unpack( packed );
  return decode_signed_octahedron( encoded_normal );
}

#endif


