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
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  vec4 pos = push_constants.world_matrix * local_pos;
  if( push_constants.fid == 0 ) {
    gl_Position = dynamic_uniforms.light_vp_matrix0 * pos;
  }
  else if( push_constants.fid == 1 ) {
    gl_Position = dynamic_uniforms.light_vp_matrix1 * pos;
  }
  else if( push_constants.fid == 2 ) {
    gl_Position = dynamic_uniforms.light_vp_matrix2 * pos;
  }
  else {
    gl_Position = dynamic_uniforms.light_vp_matrix3 * pos;
  }
}

