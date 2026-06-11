#ifndef GCT_SHADER_GBUFFER_VALUE_H
#define GCT_SHADER_GBUFFER_VALUE_H

#ifdef __cplusplus
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;
#endif

struct gbuffer_value {
  vec3 pos;
  float depth;
  vec3 normal;
  float metallic;
  float roughness;
  ivec4 input_id;
  vec4 albedo;
  vec3 emissive;
  float occlusion;
  vec3 optflow;
  vec4 shadow_level;
  vec3 tangent;
  vec2 texcoord[4];
  vec4 color[2];
};

#endif

