#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "io_with_tangent.h"
#include "scene_graph.h"
#include "global_uniforms.h"
#include "omnishadow.h"

layout(early_fragment_tests) in;

void main() {
  const primitive_resource_index_type prim =
    primitive_resource_index[ push_constants.primitive ];
  vec3 normal_ = normalize( input_normal.xyz );
  vec3 tangent_ = normalize( input_tangent.xyz );
  vec3 normal;
  vec3 tangent;
  if( prim.normal_texture != 0 ) {
    vec3 binormal = cross( tangent_, normal_ );
    mat3 its = mat3( tangent_, binormal, normal_ );
    normal = its * normalize( texture( texture_pool[ nonuniformEXT(prim.normal_texture) ], input_texcoord ).rgb * vec3( prim.normal_scale, prim.normal_scale, 1 ) * 2.0 - 1.0 );
    tangent = cross( normal, binormal );
  }
  else {
    normal = normal_;
    tangent = tangent_;
  }

  vec3 pos = input_position.xyz;
  vec4 albedo;
  if( prim.base_color_texture != 0 ) {
    albedo = texture( texture_pool[ nonuniformEXT(prim.base_color_texture) ], input_texcoord );
  }
  else {
    albedo = prim.base_color;
  }
  vec3 emissive;
  if( prim.emissive_texture != 0 ) {
    emissive = texture( texture_pool[ nonuniformEXT(prim.emissive_texture) ], input_texcoord ).rgb;
  }
  else {
    emissive = prim.emissive.rgb;
  }
  float metallic;
  float roughness;
  if( prim.metallic_roughness_texture != 0 ) {
    vec4 mr = texture( texture_pool[ nonuniformEXT(prim.metallic_roughness_texture) ], input_texcoord );
    metallic = mr.b;
    roughness = mr.g;
  }
  else {
    metallic = prim.metallic;
    roughness = prim.roughness;
  }
  float occlusion;
  if( prim.occlusion_texture != 0 ) {
    occlusion = mix( 1 - prim.occlusion_strength, 1, texture( texture_pool[ nonuniformEXT(prim.occlusion_texture) ], input_texcoord ).r );
  }
  else {
    occlusion = 1.0;
  }
  const vec4 screen_pos = input_optflow;
  const vec3 screen_pos_ = screen_pos.xyz / screen_pos.w;
  const vec4 previous_screen_pos = input_previous_position;
  const vec3 previous_screen_pos_ = previous_screen_pos.xyz / previous_screen_pos.w;
  const vec3 optflow = screen_pos_.xyz - previous_screen_pos_.xyz;
  float shadow_level0 = pcss_omni( input_position.xyz, light_pool[ global_uniforms.light ].world_position.xyz, light_pool[ global_uniforms.light ].light_size );
  float shadow_level1 = 0.0;
  float shadow_level2 = 0.0;
  float shadow_level3 = 1.0;

  if( albedo.a <= 0.0 ) discard;
  output_position = vec4( pos, gl_FragCoord.z );
  output_albedo = albedo;
  output_normal = vec4( normal.x, normal.y, normal.z, 1.0 );
  output_emissive = vec4( emissive, occlusion );
  output_material = vec4( metallic, roughness, input_id.x, input_id.y );
  output_optflow = vec4( optflow, 0.0 );
  output_shadow = vec4( shadow_level0, shadow_level1, shadow_level2, shadow_level3 );
  const uint visibility_index = instance_resource_index[ push_constants.instance ].visibility;
  visibility_pool[ visibility_index ] = 1;
}

