#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "io_with_tangent.h"
#include "constants.h"
#include "push_constants.h"
#include "lighting.h"
#include "omnishadow.h"

layout(binding = 1) uniform sampler2D base_color;
layout(binding = 2) uniform sampler2D metallic_roughness;
layout(binding = 4) uniform sampler2D occlusion;
layout(binding = 5) uniform sampler2D emissive;

void main()  {
  vec3 normal = normalize( input_normal.xyz );
  vec3 tangent = normalize( input_tangent.xyz );
  vec3 pos = input_position.xyz;
  vec4 albedo = texture( base_color, input_texcoord ) * uniforms.base_color;
  vec3 emissive = uniforms.emissive.rgb * texture( emissive, input_texcoord ).rgb;
  vec4 mr = texture( metallic_roughness, input_texcoord );
  float metalness = mr.b * uniforms.metalness;
  float roughness = mr.g * uniforms.roughness;
  float occlusion = mix( 1 - uniforms.occlusion_strength, 1, texture( occlusion, input_texcoord ).r );
  vec3 optflow = vec3( 0.0, 0.0, 0.0 );
  float shadow_level0 = pcss_omni( input_position.xyz, dynamic_uniforms.light_pos.xyz, dynamic_uniforms.light_size );
  float shadow_level1 = 1.0;
  float shadow_level2 = 1.0;
  float shadow_level3 = 1.0;

  if( albedo.a <= 0.0 ) discard;
  output_position = vec4( pos, gl_FragCoord.z );
  output_albedo = albedo;
  output_normal = vec4( normal.x, normal.y, normal.z, 1.0 );
  output_emissive = vec4( emissive, occlusion );
  output_material = vec4( metalness, roughness, 0.0, 0.0 );
  output_optflow = vec4( optflow, 0.0 );
  output_shadow = vec4( shadow_level0, shadow_level1, shadow_level2, shadow_level3 );
  output_face_normal = vec4( normalize( input_face_normal.xyz ), 1.0 );
}

