#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;

#include "push_constants.h"

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  const vec4 local_pos = vec4( input_position.xyz, 1.0 );
  const vec4 pos = local_pos;
  gl_Position = pos;
}

