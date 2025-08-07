#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 1) in vec2 input_texcoord;
layout (location = 2) in vec4 input_energy;
layout (location = 0) out vec4 output_color;

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

layout(push_constant) uniform PushConstants {
  vec2 sensor_size;
  uint texture_id;
  float lens_radius;
} push_constants;

void main()  {
  const vec3 flare = texture( texture_pool[ push_constants.texture_id ], input_texcoord ).rgb;
  output_color = vec4( max( flare - vec3( 0.01, 0.01, 0.01 ), vec3( 0.0, 0.0, 0.0 ) ) * input_energy.rgb, 1.0 );
}

