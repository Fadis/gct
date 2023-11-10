#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_position;
layout (location = 1) in vec4 input_texcoord;
layout (location = 0) out vec4 output_color;

#include "push_constants.h"

void main()  {
  float alpha =
    exp( -(input_texcoord.x*input_texcoord.x)/(2*0.3*0.3) ) *
    exp( -(input_texcoord.y*input_texcoord.y)/(2*0.3*0.3) );
  output_color = vec4( 1.0, 1.0, 1.0, alpha );
}

