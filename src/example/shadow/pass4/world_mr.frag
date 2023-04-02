#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "io.h"
#include "constants.h"
#include "push_constants.h"
#include "lighting.h"
#include "omnishadow.h"

layout(binding = 2) uniform sampler2D metallic_roughness;

void main()  {
  vec3 normal = normalize( input_normal.xyz );
  vec3 pos = input_position.xyz;
  vec3 N = normal;
  vec3 V = normalize(dynamic_uniforms.eye_pos.xyz-pos);
  vec3 L = normalize(dynamic_uniforms.light_pos.xyz-pos);
  vec4 mr = texture( metallic_roughness, input_texcoord );
  float roughness = mr.g * uniforms.roughness;
  float metallicness = mr.b * uniforms.metalness;
  vec4 diffuse_color = uniforms.base_color;
  if( diffuse_color.a <= 0.0 ) discard;
  float ambient = 1.0;
  vec3 emissive = uniforms.emissive.rgb;
  float shadow_level = pcss_omni( input_position.xyz, dynamic_uniforms.light_pos.xyz, dynamic_uniforms.light_size );
  vec3 linear = light_with_mask( L, V, N, V, N, diffuse_color.rgb, roughness, metallicness, ambient, emissive, dynamic_uniforms.light_energy, shadow_level );
  output_color = vec4( gamma(linear), diffuse_color.a );
}

