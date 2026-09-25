#ifndef GCT_SHADER_PERLIN_NOISE_COMMON_H
#define GCT_SHADER_PERLIN_NOISE_COMMON_H

#ifdef __cplusplus
#include <glm/common.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;
#endif

vec4 noise_mod289(vec4 x) {
  return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec3 noise_mod289(vec3 x) {
  return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec2 noise_mod289(vec2 x) {
  return x - floor(x * (1.0f / 289.0f)) * 289.0f;
}

vec4 noise_permute(vec4 x) {
  return noise_mod289( ( ( x * 34.0f ) + 10.0f ) * x );
}

vec3 noise_permute(vec3 x) {
  return noise_mod289( ( ( x * 34.0f ) + 10.0f ) * x );
}

vec4 noise_taylor_inv_sqrt(vec4 r) {
  return 1.79284291400159f - 0.85373472095314f * r;
}

vec2 noise_fade( vec2 t ) {
  return t * t *  t * ( t * ( t * 6.0f - 15.0f ) + 10.0f );
}


#endif

