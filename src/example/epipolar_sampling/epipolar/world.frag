#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 1) in vec2 input_texcoord;
layout (location = 0) out vec4 output_color;

layout(binding = 1) uniform sampler2D volumetric_light;
layout(binding = 2) uniform sampler2D volumetric_light_texcoord;

void main()  {
  // レイマーチの結果を記録したイメージのどの部分の値を読むべきかを得る
  const vec2 texcoord = vec2( input_texcoord.x, texture( volumetric_light_texcoord, input_texcoord ).r );
  // レイマーチの結果を記録したイメージの値を読んでこのピクセルでの出力値にする
  output_color = texture( volumetric_light, texcoord );
}


