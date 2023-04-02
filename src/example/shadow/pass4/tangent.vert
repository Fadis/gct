#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec4 input_tangent;
layout (location = 3) in vec2 input_texcoord0;

#include "push_constants.h"

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 2) out vec3 output_tangent;
layout (location = 3) out vec2 output_tex_coord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  vec4 pos = push_constants.world_matrix * local_pos;
  output_position = pos;
  vec4 local_normal = vec4( input_normal.xyz, 1.0 );
  output_normal = normalize( ( mat3(push_constants.world_matrix) * input_normal ) );
  output_tangent = normalize( ( mat3(push_constants.world_matrix) * input_tangent.xyz ) );
  output_tex_coord = input_texcoord0;
  gl_Position =
    dynamic_uniforms.projection_matrix *
    dynamic_uniforms.camera_matrix * pos;
}

