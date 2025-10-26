#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_image_load_formatted : enable

#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#include <gct/constants.h>
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>
#include <gct/lighting.h>

#include "io_with_tangent.h"

layout(push_constant) uniform PushConstants {
  uint offset;
  uint count;
  float shell_thickness;
} push_constants;

layout (location = 0) out vec4 output_color;

void main() {
  output_color = vec4( 0.0, 0.0, 1.0, 1.0 );
}

