#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;
layout (location = 0) out vec4 output_position;

#include "push_constants.h"

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  vec4 pos = push_constants.world_matrix * local_pos;
  output_position = pos;
  gl_Position = dynamic_uniforms.projection_matrix * dynamic_uniforms.camera_matrix * pos;
}


