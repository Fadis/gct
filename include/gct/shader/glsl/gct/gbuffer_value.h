#ifndef GCT_SHADER_GBUFFER_VALUE_H
#define GCT_SHADER_GBUFFER_VALUE_H

struct gbuffer_value {
  vec3 pos;
  float depth;
  vec3 normal;
  float metallic;
  float roughness;
  ivec2 input_id;
  vec4 albedo;
  vec3 emissive;
  float occlusion;
  vec3 optflow;
  vec4 shadow_level;
};

#endif

