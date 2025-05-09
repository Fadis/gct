#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_image_load_formatted : enable

layout (location = 0) in vec4 input_position;

#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph/image_pool.h>
#include <gct/global_uniforms.h>

layout(early_fragment_tests) in;

void main() {
  const ivec2 image_pos = ivec2( gl_FragCoord.x, gl_FragCoord.y );
  imageStore( image_pool_2d[ nonuniformEXT( global_uniforms.gbuffer ) ], image_pos, vec4( input_position.xyz, 1.0 ) );
}

