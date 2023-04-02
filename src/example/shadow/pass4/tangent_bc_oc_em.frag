#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "io_with_tangent.h"
#include "constants.h"
#include "push_constants.h"
#include "lighting.h"
#include "omnishadow.h"

layout(binding = 1) uniform sampler2D base_color;
layout(binding = 4) uniform sampler2D occlusion;
layout(binding = 5) uniform sampler2D emissive;

void main()  {
  vec3 normal = normalize( input_normal.xyz );
  vec3 tangent = normalize( input_tangent.xyz );
  vec3 binormal = cross( tangent, normal );
  mat3 ts = transpose( mat3( tangent, binormal, normal ) );
  vec3 pos = input_position.xyz;
  vec3 N = vec3( 0, 0, 1 );
  vec3 V = ts * normalize(dynamic_uniforms.eye_pos.xyz-pos);
  vec3 L = ts * normalize(dynamic_uniforms.light_pos.xyz-pos);
  float roughness = uniforms.roughness;
  float metallicness = uniforms.metalness;
  vec4 diffuse_color = texture( base_color, input_texcoord ) * uniforms.base_color;
  if( diffuse_color.a <= 0.0 ) discard;
  float ambient = 1.0 * mix( 1 - uniforms.occlusion_strength, 1, texture( occlusion, input_texcoord ).r );
  vec3 emissive = uniforms.emissive.rgb * texture( emissive, input_texcoord ).rgb;
  vec3 WV = normalize( dynamic_uniforms.eye_pos.xyz-pos );
  vec3 WN = normal;
  float shadow_level = pcss_omni( input_position.xyz, dynamic_uniforms.light_pos.xyz, dynamic_uniforms.light_size );
  vec3 linear = light_with_mask( L, V, N, WV, WN, diffuse_color.rgb, roughness, metallicness, ambient, emissive, dynamic_uniforms.light_energy, shadow_level );
  output_color = vec4( gamma(linear), diffuse_color.a );
}

