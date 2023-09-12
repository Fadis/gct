#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_position;
layout (location = 0) out vec4 output_position;

void main()  {
  vec3 pos = input_position.xyz;
  output_position = vec4( pos, gl_FragCoord.z );
}

