#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "io_with_tangent.h"
#include "constants.h"
#include "push_constants.h"
#include "lighting.h"

layout(binding = 4) uniform sampler2D occlusion;

void main()  {
  float ambient = mix( 1 - uniforms.occlusion_strength, 1, texture( occlusion, input_texcoord ).r );
  output_color = vec4( vec3(ambient), 1 );
}

