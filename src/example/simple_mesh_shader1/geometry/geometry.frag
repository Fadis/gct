#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 output_color;

void main() {
  // 0番目のカラーバッファを白で塗る
  output_color = vec4( 1, 1, 1, 1 );
}

