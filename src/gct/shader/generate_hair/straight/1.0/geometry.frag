#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_image_load_formatted : enable

#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#define GCT_MAKE_IMAGE_COHERENT
#include <gct/scene_graph.h>
#include <gct/scene_graph/ppll.h>

layout(push_constant) uniform PushConstants {
  float straight_factor;
  uint ppll_state_id;
  uint gbuffer_format;
  uint gbuffer;
  uint position;
  uint start;
  uint next;
} push_constants;

layout (location = 0) in float input_alpha;

void main() {
  primitive_value p;
  p.pos = vec3( gl_FragCoord.xyz );
  p.albedo = vec4( 1.0, 1.0, 1.0, input_alpha );
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
    uvec2( 0, 0 )
  );
  endInvocationInterlockARB();
}

