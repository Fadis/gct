#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 3) in vec2 input_texcoord0;

#include "push_constants.h"

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 3) out vec2 output_tex_coord;
layout (location = 4) out vec4 output_shadow0;
layout (location = 5) out vec4 output_shadow1;
layout (location = 6) out vec4 output_shadow2;
layout (location = 7) out vec4 output_shadow3;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  vec4 pos = push_constants.world_matrix * local_pos;
  output_position = pos;
  output_normal = normalize( ( mat3(push_constants.world_matrix) * input_normal ) );
  output_tex_coord = input_texcoord0;
  gl_Position = dynamic_uniforms.projection_matrix * dynamic_uniforms.camera_matrix * pos;
  output_shadow0 = dynamic_uniforms.light_vp_matrix0 * pos;
  output_shadow1 = dynamic_uniforms.light_vp_matrix1 * pos;
  output_shadow2 = dynamic_uniforms.light_vp_matrix2 * pos;
  output_shadow3 = dynamic_uniforms.light_vp_matrix3 * pos;
}

