#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable

#include "io_with_tangent.h"
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

//layout(early_fragment_tests) in;

void main() {
  primitive_value p = read_primitive(
    push_constants.primitive,
    input_position,
    input_normal,
    input_tangent,
    input_texcoord,
    input_optflow,
    input_previous_position
  );
  
  if( p.albedo.a <= 0.0 ) discard;

  float shadow_level0 = 0.0;
  float shadow_level1 = 0.0;
  float shadow_level2 = 0.0;
  float shadow_level3 = 1.0;

  const uint visibility_index = instance_resource_index[ push_constants.instance ].visibility;
  visibility_pool[ visibility_index ] = 1;

  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  beginInvocationInterlockARB();
  update_kplus_buffer16(
    global_uniforms.gbuffer,
    global_uniforms.depth,
    image_pos,
    p,
    gl_FragCoord.z,
    input_id.xy,
    vec4( shadow_level0, shadow_level1, shadow_level2, shadow_level3 )
  );
  endInvocationInterlockARB();
}

