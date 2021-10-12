#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "io.h"
#include "constants.h"
#include "push_constants.h"
#include "lighting.h"

layout(binding = 1) uniform sampler2D base_color;
layout(binding = 2) uniform sampler2D metallic_roughness;
layout(binding = 5) uniform sampler2D emissive;

void main()  {
  vec3 normal = normalize( input_normal.xyz );
  vec3 pos = input_position.xyz;
  vec3 N = normal;
  vec3 V = normalize(dynamic_uniforms.eye_pos.xyz-pos);
  vec3 L = normalize(dynamic_uniforms.light_pos.xyz-pos);
  vec4 mr = texture( metallic_roughness, input_texcoord );
  float roughness = mr.g * uniforms.roughness;
  float metallicness = mr.b * uniforms.metalness;
  vec4 diffuse_color = texture( base_color, input_texcoord ) * uniforms.base_color;
  float ambient = 0.05;
  vec3 emissive = uniforms.emissive.rgb * texture( emissive, input_texcoord ).rgb;
  vec3 linear = light( L, V, N, diffuse_color.rgb, roughness, metallicness, ambient, emissive, dynamic_uniforms.light_energy );
  output_color = vec4( gamma(linear), diffuse_color.a );
}

