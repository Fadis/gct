#version 450
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

layout(push_constant) uniform PushConstants {
  vec2 sensor_size;
  uint texture_id;
  float lens_radius;
} push_constants;


layout (location = 1) in vec2 input_texcoord;
layout (location = 2) in vec4 input_energy;
layout (location = 0) out vec4 output_color;

void main()  {
  // テクスチャから絞りのパターンを読んでビルボードの範囲に貼り付ける
  const vec3 flare = texture( texture_pool[ push_constants.texture_id ], input_texcoord ).rgb;
  output_color = vec4( flare * input_energy.rgb, 1.0 );
}

