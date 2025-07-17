#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) out vec4 output_depth;

#define GCT_MAKE_IMAGE_COHERENT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>
#include <gct/depth.h>

layout(early_fragment_tests) in;

layout (std430, binding = 13) buffer AFState {
  uint history;
  uint max_history;
  float depth;
  float znear;
  float zfar;
  float focal_distance;
  float lens_size;
  float sensor_size;
  float apeture_rotation;
  float visible_range;
} af_state;

void main() {
  const float linear_depth = decode_depth( gl_FragCoord.z, af_state.znear, af_state.zfar );
  output_depth = vec4( linear_depth/*gl_FragCoord.z*/, 0, 0, 1 );
}

