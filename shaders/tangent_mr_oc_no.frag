#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "io_with_tangent.h"
#include "constants.h"
#include "push_constants.h"
#include "lighting.h"

layout(binding = 2) uniform sampler2D metallic_roughness;
layout(binding = 3) uniform sampler2D normal_map;
layout(binding = 4) uniform sampler2D occlusion;

void main()  {
  vec3 normal = normalize( input_normal.xyz );
  vec3 tangent = normalize( input_tangent.xyz );
  vec3 binormal = cross( tangent, normal );
  mat3 ts = transpose( mat3( tangent, binormal, normal ) );
  vec3 pos = input_position.xyz;
  vec3 N = normalize( texture( normal_map, input_texcoord ).rgb * vec3( uniforms.normal_scale, uniforms.normal_scale, 1 ) * 2.0 - 1.0 );
  vec3 V = ts * normalize(dynamic_uniforms.eye_pos.xyz-pos);
  vec3 L = ts * normalize(dynamic_uniforms.light_pos.xyz-pos);
  vec4 mr = texture( metallic_roughness, input_texcoord );
  float roughness = mr.g * uniforms.roughness;
  float metallicness = mr.b * uniforms.metalness;
  vec4 diffuse_color = uniforms.base_color;
  float ambient = 0.05 * mix( 1 - uniforms.occlusion_strength, 1, texture( occlusion, input_texcoord ).r );
  vec3 emissive = uniforms.emissive.rgb;
  vec3 linear = light( L, V, N, diffuse_color.rgb, roughness, metallicness, ambient, emissive, dynamic_uniforms.light_energy );
  output_color = vec4( gamma(linear), diffuse_color.a );
}

