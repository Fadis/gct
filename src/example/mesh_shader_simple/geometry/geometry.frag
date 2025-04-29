#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_ARB_fragment_shader_interlock : enable

layout (location = 0) out vec4 output_color;

//layout(early_fragment_tests) in;

void main() {
  output_color = vec4( 1, 0, 0, 1 );
}

