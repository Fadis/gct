#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include <gct/scene_graph.h>
#include "global_uniforms.h"

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 3) in vec2 input_texcoord0;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  mat4 world_matrix = matrix_pool[ instance_resource_index[ push_constants.instance ].world_matrix ];
  vec4 pos = world_matrix * local_pos;
  gl_Position =
    matrix_pool[ global_uniforms.projection_matrix ] *
    matrix_pool[ global_uniforms.camera_matrix ] * pos;
}

