#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 output_color;
layout (location = 0) in vec4 input_position;

layout(early_fragment_tests) in;

void main()  {
  vec4 p = input_position / input_position.w;
  float distance2 = dot( p, p );
  float distance = sqrt( distance2 );
  output_color = vec4( distance, 0, 0, 0 );
}

