#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 1) in vec2 input_texcoord;
layout (location = 2) in vec4 input_energy;
layout (location = 0) out vec4 output_color;

#include "scene_graph.h"
#include "global_uniforms.h"

void main()  {
  const vec3 flare = texture( texture_pool[ push_constants.texture_id ], input_texcoord ).rgb;
  output_color = vec4( flare * input_energy.rgb, 1.0 );
}

