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

//layout(early_fragment_tests) in;

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  uint light;
  uint shell_thickness;
  uint loop_counter;
  uint loop_until;
  uint ppll_state_id;
  uint gbuffer;
  uint gbuffer_format;
  uint position;
  uint start;
  uint next;
} push_constants;


void main() {
  primitive_value p = read_primitive(
    uint( input_id.y ),
    input_position,
    input_normal,
    input_tangent,
    input_texcoord,
    input_optflow,
    input_previous_position
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
    GCT_GBUFFER_ALBEDO_ALPHA |
    GCT_GBUFFER_TANGENT |
    GCT_GBUFFER_EMISSIVE_OCCLUSION |
    GCT_GBUFFER_METALLIC_ROUGHNESS_ID |
    GCT_GBUFFER_OPTFLOW_MARK
  );
  ppll_insert(
    iter,
    p,
    gl_FragCoord.z,
    input_id.xy
  );
  endInvocationInterlockARB();
}

