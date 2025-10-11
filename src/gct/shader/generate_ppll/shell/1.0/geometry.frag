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

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  float shell_thickness;
  uint loop_counter;
  uint loop_until;
  uint shell_texture;
  float shell_texture_clamp_min;
  float shell_texture_clamp_max;
  uint ppll_state_id;
  uint gbuffer;
  uint gbuffer_format;
  uint position;
  uint start;
  uint next;
} push_constants;

layout(early_fragment_tests) in;

primitive_value read_primitive(
  uint primitive_id,
  vec4 vert_position,
  vec3 vert_normal,
  vec2 vert_texcoord0,
  vec2 vert_texcoord1,
  uint shell_texture,
  float relative_shell_height,
  float shell_texture_clamp_min,
  float shell_texture_clamp_max
) {
  const primitive_resource_index_type prim =
    primitive_resource_index[ primitive_id ];
  const vec3 normal = normalize( vert_normal.xyz );
  const vec3 pos = vert_position.xyz;
  const vec3 albedo = (
    ( prim.base_color_texture != 0 ) ?
    texture( texture_pool[ nonuniformEXT(prim.base_color_texture) ], vert_texcoord0 ) :
    prim.base_color
  ).xyz;

  const float shell_texture_value = texture(
    texture_pool[ nonuniformEXT( shell_texture ) ],
    vert_texcoord1
  ).r;

  const float height = min(
    max(
      shell_texture_value - push_constants.shell_texture_clamp_min,
      0.0
    )/
    ( push_constants.shell_texture_clamp_max - push_constants.shell_texture_clamp_min ),
    1.0
  );

  const float width0 = 1.0 - ( height - 1.0/16.0 );
  const float width1 = 1.0 - height;
  const float coverage =
    ( height >= relative_shell_height ) ?
    ( width0 + width1 ) / 2.0 :
    0.0;


  const float alpha = ( height >= relative_shell_height ) ? 1.0 : 0.0;
  const vec3 emissive =
    ( prim.emissive_texture != 0 ) ?
    texture( texture_pool[ nonuniformEXT(prim.emissive_texture) ], vert_texcoord0 ).rgb :
    prim.emissive.rgb;
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
  float occlusion = relative_shell_height * 0.5 + 0.5;

  primitive_value temp;
  temp.pos = pos;
  temp.tangent = normal;
  temp.albedo = vec4( albedo, coverage );
  temp.emissive = emissive;
  temp.metallic = metallic;
  temp.roughness = roughness;
  temp.occlusion = occlusion;
  return temp;
}

void main() {
  primitive_value p = read_primitive(
    uint( input_id.y ),
    input_position,
    input_normal,
    input_texcoord0,
    input_texcoord1,
    push_constants.shell_texture,
    input_relative_height,
    push_constants.shell_texture_clamp_min,
    push_constants.shell_texture_clamp_max
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

