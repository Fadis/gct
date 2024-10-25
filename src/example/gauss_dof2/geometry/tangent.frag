#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable

#include "io_with_tangent.h"
#include "scene_graph.h"
#include "global_uniforms.h"
#include "omnishadow.h"

//layout(early_fragment_tests) in;

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
  vec4 screen_pos = input_optflow;
  vec3 screen_pos_ = screen_pos.xyz / screen_pos.w;
  vec4 previous_screen_pos = input_previous_position;
  vec3 previous_screen_pos_ = previous_screen_pos.xyz / previous_screen_pos.w;
  vec3 optflow = screen_pos_.xyz - previous_screen_pos_.xyz;
  float shadow_level0 = pcss_omni( input_position.xyz, light_pool[ global_uniforms.light ].world_position.xyz, light_pool[ global_uniforms.light ].light_size );
  float shadow_level1 = 0.0;
  float shadow_level2 = 0.0;
  float shadow_level3 = 1.0;

  if( albedo.a <= 0.0 ) discard;
  output_position = vec4( pos, gl_FragCoord.z );
  const uint visibility_index = instance_resource_index[ push_constants.instance ].visibility;
  visibility_pool[ visibility_index ] = 1;
  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  beginInvocationInterlockARB();
  ivec4 sample_index = ivec4( imageLoad( extended_gbuffer, ivec3( image_pos, 32 ) ) );
  const vec4 sample_depth = vec4(
    sample_index.x != 0 ?
      imageLoad( extended_gbuffer, ivec3( image_pos, ( sample_index.x - 1 ) * 8 ) ).w :
      2.0,
    sample_index.y != 0 ?
      imageLoad( extended_gbuffer, ivec3( image_pos, ( sample_index.y - 1 ) * 8 ) ).w :
      2.0,
    sample_index.z != 0 ?
      imageLoad( extended_gbuffer, ivec3( image_pos, ( sample_index.z - 1 ) * 8 ) ).w :
      2.0,
    sample_index.w != 0 ?
      imageLoad( extended_gbuffer, ivec3( image_pos, ( sample_index.w - 1 ) * 8 ) ).w :
      2.0
  );
  const uint existing_sample_count =
    ( sample_index.x != 0 ? 1 : 0 ) +
    ( sample_index.y != 0 ? 1 : 0 ) +
    ( sample_index.z != 0 ? 1 : 0 ) +
    ( sample_index.w != 0 ? 1 : 0 );
  uint new_sample_index =
    ( existing_sample_count >= 4 ) ?
      uint( sample_index.w ) :
      existing_sample_count + 1;
  const uint new_sample_pos =
    ( gl_FragCoord.z > sample_depth.x ? 1 : 0 ) +
    ( gl_FragCoord.z > sample_depth.y ? 1 : 0 ) +
    ( gl_FragCoord.z > sample_depth.z ? 1 : 0 ) +
    ( gl_FragCoord.z > sample_depth.w ? 1 : 0 );
  sample_index = (
    new_sample_pos == 0 ?
    ivec4( new_sample_index, sample_index.x, sample_index.y, sample_index.z ) :
    (
      new_sample_pos == 1 ?
      ivec4( sample_index.x, new_sample_index, sample_index.y, sample_index.z ) :
      (
        new_sample_pos == 2 ?
        ivec4( sample_index.x, sample_index.y, new_sample_index, sample_index.z ) :
        (
          new_sample_pos == 3 ?
          ivec4( sample_index.x, sample_index.y, sample_index.z, new_sample_index ) :
          ivec4( sample_index.x, sample_index.y, sample_index.z, sample_index.w )
        )
      )
    )
  );
  if( new_sample_pos < 4 ) {
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 0 ), vec4( pos, gl_FragCoord.z ) );
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 1 ), albedo );
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 2 ), vec4( normal.x, normal.y, normal.z, 1.0 ) );
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 3 ), vec4( emissive, occlusion ) );
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 4 ), vec4( metallic, roughness, input_id.x, input_id.y ) );
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 5 ), vec4( optflow, 0.0 ) );
    imageStore( extended_gbuffer, ivec3( image_pos, ( new_sample_index - 1 ) * 8 + 6 ), vec4( shadow_level0, shadow_level1, shadow_level2, shadow_level3 ) );
    imageStore( extended_gbuffer, ivec3( image_pos, 32 ), vec4( sample_index ) );
  }
  endInvocationInterlockARB();
}

