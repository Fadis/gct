#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) out vec4 output_color;

//layout(early_fragment_tests) in;

void main() {
  output_color = vec4( 1, 0, 0, 1 );
}

