#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_color;
layout (location = 0) out vec4 output_color;

void main()  {
  output_color = vec4( input_color, 1.0 );
}


