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
#include <gct/global_uniforms.h>

//layout(early_fragment_tests) in;

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  uint gbuffer_format;
  uint gbuffer;
  uint position;
} push_constants;

void main() {
  rasterizable_vertex_attribute p;
  p.position = input_position;
  p.normal = input_normal.xyz;
  p.tangent = input_tangent;
  p.texcoord = vec3( input_texcoord, 0.0 );
  /*
  p.normal *= 0.5;
  p.normal += 0.5;
  p.tangent.xzy *= 0.5;
  p.tangent.xyz += 0.5;
  p.tangent.w = 1.0;
*/
  const uint visibility_index = instance_resource_index[ uint( input_id.x ) ].visibility;
  visibility_pool[ visibility_index ] = 1;

  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  beginInvocationInterlockARB();
  gbuffer_iter iter = gbuffer_begin(
    gbuffer_image( push_constants.gbuffer, push_constants.position ),
    image_pos,
    push_constants.gbuffer_format
  );
  gbuffer_insert_lazy(
    iter,
    p,
    gl_FragCoord.z,
    input_id
    //vec4( 0, 1, 1, 1 )
  );
  endInvocationInterlockARB();
}

