#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_image_load_formatted : enable

#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

layout (points) in;
layout (triangle_strip,max_vertices=18) out;

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

layout(push_constant) uniform PushConstants {
  vec2 sensor_size;
  uint texture_id;
  float lens_radius;
} push_constants;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 1) out vec2 output_texcoord;
layout (location = 2) out vec4 output_energy;

void main() {
  vec4 light_pos_in_screen = gl_in[ 0 ].gl_Position;
  light_pos_in_screen/=light_pos_in_screen.w;
  ivec2 screen_pos = ivec2( imageSize( image_pool_2d[ global_uniforms.gbuffer ] ).xy * ( light_pos_in_screen.xy * vec2( 0.5,  0.5 ) + 0.5 ) );
  vec4 layer_index = imageLoad( image_pool_2d_array[ nonuniformEXT( global_uniforms.gbuffer ) ], ivec3( screen_pos, 32 ) );
  const float depth = 
    ( layer_index.x != 0 ) ?
    read_kplus_buffer_depth( global_uniforms.depth, screen_pos, uint( layer_index.x ) ):
    2.0;
  if(
    light_pos_in_screen.x < -1.0 || light_pos_in_screen.x > 1.0 ||
    light_pos_in_screen.y < -1.0 || light_pos_in_screen.y > 1.0 ||
   light_pos_in_screen.z < -1.0 || light_pos_in_screen.z > depth
  ) return;
  const float radius = push_constants.lens_radius;
  const vec2 center_pos = light_pos_in_screen.xy;
  const vec2 v0 = center_pos + vec2( -radius,  radius )/push_constants.sensor_size;
  const vec2 v1 = center_pos + vec2( -radius, -radius )/push_constants.sensor_size;
  const vec2 v2 = center_pos + vec2(  radius,  radius )/push_constants.sensor_size;
  const vec2 v3 = center_pos + vec2(  radius, -radius )/push_constants.sensor_size;
  const vec4 energy = vec4(
    light_pool[ global_uniforms.light ].energy.xyz,
    1.0
  );
  gl_Position = vec4( v0, 0.0, 1.0 );
  output_texcoord = vec2( 0.0, 1.0 );
  output_energy = energy;
  EmitVertex();
  gl_Position = vec4( v1, 0.0, 1.0 );
  output_texcoord = vec2( 0.0, 0.0 );
  output_energy = energy;
  EmitVertex();
  gl_Position = vec4( v2, 0.0, 1.0 );
  output_texcoord = vec2( 1.0, 1.0 );
  output_energy = energy;
  EmitVertex();
  EndPrimitive();

  gl_Position = vec4( v2, 0.0, 1.0 );
  output_texcoord = vec2( 1.0, 1.0 );
  output_energy = energy;
  EmitVertex();
  gl_Position = vec4( v1, 0.0, 1.0 );
  output_texcoord = vec2( 0.0, 0.0 );
  output_energy = energy;
  EmitVertex();
  gl_Position = vec4( v3, 0.0, 1.0 );
  output_texcoord = vec2( 1.0, 0.0 );
  output_energy = energy;
  EmitVertex();
  EndPrimitive();
}

