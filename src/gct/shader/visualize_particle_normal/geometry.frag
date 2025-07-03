#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable

#define GCT_MAKE_IMAGE_COHERENT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

//layout(early_fragment_tests) in;

void main() {
  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  beginInvocationInterlockARB();
  mark_kplus_buffer16(
    global_uniforms.gbuffer,
    image_pos
  );
  endInvocationInterlockARB();
}

