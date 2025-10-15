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
#include <gct/scene_graph/read_hair_primitive.h>

layout(early_fragment_tests) in;

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  float shell_thickness;
  uint fin_texture;
  uint ppll_state_id;
  uint gbuffer;
  uint gbuffer_format;
  uint position;
  uint start;
  uint next;
} push_constants;

primitive_value read_primitive_hair(
  uint primitive_id,
  vec4 vert_position,
  vec3 vert_tangent,
  vec2 vert_texcoord0,
  vec2 vert_texcoord1,
  uint fin_texture
) {
  const primitive_resource_index_type prim =
    primitive_resource_index[ primitive_id ];
  const vec3 tangent = normalize( vert_tangent.xyz );
  const vec3 pos = vert_position.xyz;
  const vec3 albedo = (
    ( prim.base_color_texture != 0 ) ?
    texture( texture_pool[ nonuniformEXT(prim.base_color_texture) ], vert_texcoord0 ) :
    prim.base_color
  ).xyz;
  const float alpha =
    texture( texture_pool[ nonuniformEXT( fin_texture) ], vert_texcoord1 ).r;

  const float occlusion = vert_texcoord1.y * 0.5 + 0.5;

  float metallic;
  float roughness;
  if( prim.metallic_roughness_texture != 0 ) {
    vec4 mr = texture( texture_pool[ nonuniformEXT(prim.metallic_roughness_texture) ], vert_texcoord0 );
    metallic = mr.b;
    roughness = mr.g;
  }
  else {
    metallic = prim.metallic;
    roughness = prim.roughness;
  }

  primitive_value temp;
  temp.pos = pos;
  temp.tangent = tangent;
  temp.albedo = vec4( albedo, alpha );
  temp.occlusion = occlusion;
  temp.metallic = metallic;
  temp.roughness = roughness;
  temp.texcoord[ 0 ] = vert_texcoord0;
  temp.texcoord[ 1 ] = vert_texcoord1;
  return temp;
}


void main() {
  primitive_value p = read_primitive_hair(
    uint( input_id.y ),
    input_position,
    input_tangent,
    input_texcoord0,
    input_texcoord1,
    push_constants.fin_texture
  );
 
  if( p.albedo.a <= 0.0 ) discard;

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

