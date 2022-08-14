#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;
layout (location = 2) in vec3 input_color;
layout (location = 0) out vec3 output_color;
out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  output_color = input_color;
  gl_Position = vec4( input_position, 1.0 );
}

