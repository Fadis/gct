#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "scene_graph.h"
#include "global_uniforms.h"

layout (location = 0) in vec2 input_position;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  const vec3 pos = light_pool[ global_uniforms.light ].world_position.xyz;
  gl_Position =
    matrix_pool[ global_uniforms.projection_matrix ] *
    matrix_pool[ global_uniforms.camera_matrix ] * vec4( pos, 1.0 );
}

