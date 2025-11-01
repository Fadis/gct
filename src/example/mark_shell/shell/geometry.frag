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
  uint light;
  float shell_thickness;
  uint loop_counter;
  uint loop_until;
} push_constants;

layout (location = 0) out vec4 output_color;

layout(early_fragment_tests) in;

void main() {
  primitive_value p = read_primitive(
    uint( input_id.y ),
    input_position,
    input_normal,
    input_tangent,
    input_texcoord,
    input_optflow,
    input_previous_position
  );
  
  const vec3 V = normalize(global_uniforms.eye_pos.xyz-p.pos);
  const vec3 L = normalize(light_pool[ push_constants.light ].world_position.xyz-p.pos);

  const vec3 diffuse = diffuse_with_mask(
    L, V, p.normal, p.albedo.rgb, p.roughness, p.metallic, p.emissive,
    light_pool[ push_constants.light ].energy.xyz,
    1.0
  );
  
  const vec3 specular = specular_with_mask(
    L, V, p.normal, p.albedo.rgb, p.roughness, p.metallic,
    light_pool[ push_constants.light ].energy.xyz,
    1.0
  );

  const vec3 ambient = global_uniforms.ambient * p.albedo.rgb;

  //output_color = vec4( ambient + diffuse + specular, p.albedo.a );
  output_color = vec4( 0.0, 1.0, 0.0, 0.05 );
}

