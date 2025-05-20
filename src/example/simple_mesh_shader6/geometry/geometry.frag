#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

// 0番目の頂点属性を補間した物
layout (location = 0) in vec4 input_color;

// ここにピクセルの色を出力する
layout (location = 0) out vec4 output_color;

void main() {
  // ピクセルの色を頂点属性から渡ってきた色にする
  output_color = input_color;
}

