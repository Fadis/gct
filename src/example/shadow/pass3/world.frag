#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#include "push_constants.h"
#include "constants.h"
#include "omnishadow.h"

layout (location = 0) in vec4 input_position;
layout (location = 0) out vec4 output_color;

void main() {
  float r = pcss_omni( input_position.xyz, dynamic_uniforms.light_pos.xyz, dynamic_uniforms.light_size );
  output_color = vec4( r, r, r, 1.0 );
}

