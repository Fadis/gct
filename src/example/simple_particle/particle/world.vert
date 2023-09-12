#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;

#include "push_constants.h"

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec4 output_scale;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  vec4 pos = local_pos;
  output_position = pos;
  vec4 camera_space_pos = dynamic_uniforms.camera_matrix * pos;
  // 投影行列に設定された値に基づいて遠くの点ほど小さいビルボードが描かれるようにする
  const float scale = camera_space_pos.z * dynamic_uniforms.projection_matrix[ 3 ][ 2 ];
  // アスペクト比で割って画面上でビルボードが正方形になるようにする
  const float aspect = 9.0/16.0;
  output_scale = vec4( 1.0*aspect/scale, 1.0/scale, 0.0, 1.0 );
  gl_Position = dynamic_uniforms.projection_matrix * dynamic_uniforms.camera_matrix * pos;
}

