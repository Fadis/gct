#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_texcoord;
layout (location = 1) in float input_linear_z;
layout (location = 0) out vec4 output_coef0;
layout (location = 1) out vec4 output_coef1;

layout(push_constant) uniform PushConstants {
  float near;
  float far;
  float aspect;
  float point_size;
  uint split_count;
  float color;
} push_constants;

#include "constants.h"
#include "push_constants.h"
#include "random.h"

void main()  {
  const float eps = 1.0e-3;
  const float alpha =
    exp( -(input_texcoord.x*input_texcoord.x)/(2*0.3*0.3) ) *
    exp( -(input_texcoord.y*input_texcoord.y)/(2*0.3*0.3) ) *
    rand1( input_texcoord.xy ) *
    push_constants.color;
  float a0 = -2.0 * log( max( 1.0 - alpha, eps ) );
  float a1 = a0 * cos( 2.0 * pi * 1.0 * input_linear_z );
  float a2 = a0 * cos( 2.0 * pi * 2.0 * input_linear_z );
  float a3 = a0 * cos( 2.0 * pi * 3.0 * input_linear_z );
  float b1 = a0 * sin( 2.0 * pi * 1.0 * input_linear_z );
  float b2 = a0 * sin( 2.0 * pi * 2.0 * input_linear_z );
  float b3 = a0 * sin( 2.0 * pi * 3.0 * input_linear_z );
  output_coef0 = vec4( a0, a1, a2, a3 );
  output_coef1 = vec4( 0.0, b1, b2, b3 );
}

