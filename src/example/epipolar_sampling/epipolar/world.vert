#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec2 input_position;
layout (location = 1) in vec2 input_texcoord;
layout (location = 1) out vec2 output_texcoord;
out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  // 求めたメッシュは元々スクリーン空間の座標なので、変換せずにそのままラスタライズにまわす
  gl_Position = vec4( input_position, 0.0, 1.0 );
  output_texcoord = input_texcoord;
}

