#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable
#extension GL_EXT_shader_image_load_formatted : enable

#include "io_with_tangent.h"
#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#define GCT_MAKE_IMAGE_COHERENT
#include <gct/scene_graph.h>
#include <gct/scene_graph/ppll.h>
#include <gct/global_uniforms.h>


//layout(early_fragment_tests) in;

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  uint ppll_state_id;
  uint gbuffer_format;
  uint gbuffer;
  uint position;
  uint start;
  uint next;
  uint hair_texture;
} push_constants;


primitive_value read_hair_primitive(
  uint primitive_id,
  vec4 vert_position,
  vec3 vert_normal,
  vec3 vert_tangent,
  vec2 vert_texcoord,
  vec2 vert_hair_texcoord,
  vec4 vert_optflow,
  vec4 vert_previous_position,
  uint hair_texture
) {
  const primitive_resource_index_type prim =
    primitive_resource_index[ primitive_id ];
  const vec3 normal_ = normalize( vert_normal.xyz );
  const vec3 tangent_ = cross( normal_, normalize( vert_tangent.xyz ) );

  const vec3 pos = vert_position.xyz;
  vec4 albedo =
    ( prim.base_color_texture != 0 ) ?
    texture( texture_pool[ nonuniformEXT(prim.base_color_texture) ], vert_texcoord ) :
    prim.base_color;
  const vec3 emissive =
    ( prim.emissive_texture != 0 ) ?
    texture( texture_pool[ nonuniformEXT(prim.emissive_texture) ], vert_texcoord ).rgb :
    prim.emissive.rgb;
  float metallic;
  float roughness;
  if( prim.metallic_roughness_texture != 0 ) {
    vec4 mr = texture( texture_pool[ nonuniformEXT(prim.metallic_roughness_texture) ], vert_texcoord );
    metallic = mr.b;
    roughness = mr.g;
  }
  else {
    metallic = prim.metallic;
    roughness = prim.roughness;
  }
  const vec4 screen_pos = vert_optflow;
  const vec3 screen_pos_ = screen_pos.xyz / screen_pos.w;
  const vec4 previous_screen_pos = vert_previous_position;
  const vec3 previous_screen_pos_ = previous_screen_pos.xyz / previous_screen_pos.w;
  const vec3 optflow = screen_pos_.xyz - previous_screen_pos_.xyz;

  const vec2 hair_param = texture( texture_pool[ nonuniformEXT(hair_texture) ], vert_hair_texcoord ).rg;

  const float occlusion = 1.0 - hair_param.r;

  albedo.a *= hair_param.g;

  primitive_value temp;
  temp.pos = pos;
  temp.albedo = albedo;
  temp.emissive = emissive;
  temp.metallic = metallic;
  temp.roughness = roughness;
  temp.occlusion = occlusion;
  temp.optflow = optflow;
  temp.tangent = tangent_;
  temp.texcoord[ 0 ] = vert_texcoord;
  return temp;
}

void main() {
  primitive_value p = read_hair_primitive(
    uint( input_id.y ),
    input_position,
    input_normal,
    input_tangent,
    input_texcoord,
    input_texcoord,
    input_optflow,
    input_previous_position,
    push_constants.hair_texture
  );
  
  if( p.albedo.a <= 0.0 ) discard;

  const uint visibility_index = instance_resource_index[ uint( input_id.x ) ].visibility;
  visibility_pool[ visibility_index ] = 1;

  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  
  beginInvocationInterlockARB();
  ppll_iter iter = ppll_begin(
    push_constants.ppll_state_id,
    ppll_image( push_constants.gbuffer, push_constants.position, push_constants.start, push_constants.next ),
    image_pos,
    push_constants.gbuffer_format
  );
  ppll_insert(
    iter,
    p,
    gl_FragCoord.z,
    input_id.xy
  );
  endInvocationInterlockARB();
}

