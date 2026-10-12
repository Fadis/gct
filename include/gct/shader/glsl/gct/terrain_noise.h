#ifndef GCT_SHADER_TERRAIN_NOISE_H
#define GCT_SHADER_TERRAIN_NOISE_H

#ifdef __cplusplus
#include <gct/shader/glsl/gct/simplex_noise.h>
#include <gct/shader/glsl/gct/cellular_noise.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;
#else
#include <gct/simplex_noise.h>
#include <gct/cellular_noise.h>
#endif

float noise_terrain2d( vec2 P ) {
  vec2 x_offset = vec2(
    2.818472461772784f,
    5.156369095321605f
  );
  vec2 y_offset = vec2(
    6.836664965664836f,
    1.782568237626052f
  );
  vec2 Q = P + vec2(
    noise_simplex2d( P + x_offset ),
    noise_simplex2d( P + y_offset )
  ) * 0.25f * 0.125f;
  vec2 c = noise_cellular2d( Q );
  float s = noise_simplex2d_fbm10( Q );
  return ( -c.x + c.y ) / 3.0f + s * 2.0f / 3.0f;
}

float noise_terrain2d( vec2 P, float ref ) {
  vec2 x_offset = vec2(
    2.818472461772784f,
    5.156369095321605f
  );
  vec2 y_offset = vec2(
    6.836664965664836f,
    1.782568237626052f
  );
  vec2 Q = P + vec2(
    noise_simplex2d( P + x_offset ),
    noise_simplex2d( P + y_offset )
  ) * 0.25f;
  vec2 c = noise_cellular2d( Q );
  float s = noise_simplex2d_fbm10( Q );
  return ( -c.x + c.y ) / 3.0f + s / 3.0f + ref / 3.0f;
}



#endif

