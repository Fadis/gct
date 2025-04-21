#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable

#include "io.h"
#include <gct/global_uniforms.h>
#include <gct/scene_graph.h>

//layout(early_fragment_tests) in;

void main()  {
  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  const primitive_value p =
    read_primitive(
      push_constants.primitive,
      input_position,
      input_normal,
      input_texcoord,
      input_optflow,
      input_previous_position
    );
  
  if( p.albedo.a <= 0.0 ) discard;

  const uint visibility_index = instance_resource_index[ push_constants.instance ].visibility;
  visibility_pool[ visibility_index ] = 1;

  beginInvocationInterlockARB();
  update_kplus_buffer16_reduced(
    global_uniforms.gbuffer,
    global_uniforms.depth,
    image_pos,
    p,
    gl_FragCoord.z,
    input_id.xy,
    vec4( 0.0, 0.0, 0.0, 0.0 )
  );
  endInvocationInterlockARB();
}

