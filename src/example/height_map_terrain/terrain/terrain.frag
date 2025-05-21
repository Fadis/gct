#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable

#define GCT_MAKE_IMAGE_COHERENT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>


//layout(early_fragment_tests) in;

layout (location = 0) in vec4 input_position;
layout (location = 1) in vec3 input_normal;

void main() {

  primitive_value p;
  p.pos = input_position.xyz;
  p.normal = input_normal.xyz;

  p.albedo = texture( texture_pool_2d[ nonuniformEXT( global_uniforms.terrain_tex ) ], input_position.xz / 30.0 );
  
  p.emissive = vec3( 0, 0, 0 );
  p.metallic = 0;
  p.roughness = 0.9;
  p.occlusion = 1.0;
  p.optflow = vec3( 0, 0, 0 );
  
  //if( p.albedo.a <= 0.0 ) discard;

  float shadow_level0 = 0.0;
  float shadow_level1 = 0.0;
  float shadow_level2 = 0.0;
  float shadow_level3 = 1.0;

  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  beginInvocationInterlockARB();
  update_kplus_buffer16(
    global_uniforms.gbuffer,
    global_uniforms.depth,
    image_pos,
    p,
    gl_FragCoord.z,
    vec2( 0, 0 ),
    vec4( 0, 0, 0, 0 )
  );
  endInvocationInterlockARB();
}

