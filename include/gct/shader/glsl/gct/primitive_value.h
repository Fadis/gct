#ifndef GCT_SHADER_PRIMITIVE_VALUE_H
#define GCT_SHADER_PRIMITIVE_VALUE_H

struct primitive_value {
  vec3 pos;
  vec3 normal;
  vec4 albedo;
  vec3 emissive;
  float metallic;
  float roughness;
  float occlusion;
  vec3 optflow;
};

#endif

