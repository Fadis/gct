#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable

layout (points) in;
layout (triangle_strip,max_vertices=18) out;

#include "scene_graph.h"
#include "global_uniforms.h"

layout (set = 1, binding = 19, rgba32f) uniform image2DArray gbuffer;


const uint flare_matrix_count = 36;
const mat2 flare_matrices[36]=mat2[](
#include "flare.inc"
);

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec2 input_light0[];
layout (location = 1) in vec2 input_light1[];
layout (location = 2) in vec2 input_flare_direction[];
layout (location = 3) flat in uint output_index[];

layout (location = 1) out vec2 output_texcoord;
layout (location = 2) out vec4 output_energy;

void main() {
  vec4 light_pos_in_screen = gl_in[ 0 ].gl_Position;
  light_pos_in_screen/=light_pos_in_screen.w;
  ivec2 screen_pos = ivec2( imageSize( gbuffer ).xy * ( light_pos_in_screen.xy * vec2( 0.5, -0.5 ) + 0.5 ) );
  const vec3 normal = imageLoad( gbuffer, ivec3( screen_pos, 2 ) ).xyz;
  const float depth = dot( normal, normal ) > 0.0 ? imageLoad( gbuffer, ivec3( screen_pos, 0 ) ).w : 10.0;
  if(
    light_pos_in_screen.x < -1.0 || light_pos_in_screen.x > 1.0 ||
    light_pos_in_screen.y < -1.0 || light_pos_in_screen.y > 1.0 ||
    light_pos_in_screen.z < -1.0 || light_pos_in_screen.z > depth
  ) return;
  const uint i = output_index[ 0 ];
  {
    const float r0 = ( flare_matrices[ i ] * input_light0[ 0 ] ).x;
    const float r1 = ( flare_matrices[ i ] * input_light1[ 0 ] ).x;
    const float center = ( r0 + r1 )*0.5;
    const float radius = abs( r0 - r1 )*0.5;
    const vec2 center_pos = input_flare_direction[ 0 ] * center;
    const vec2 v0 = ( center_pos + vec2( -radius,  radius ) ) / push_constants.sensor_size;
    const vec2 v1 = ( center_pos + vec2( -radius, -radius ) ) / push_constants.sensor_size;
    const vec2 v2 = ( center_pos + vec2(  radius,  radius ) ) / push_constants.sensor_size;
    const vec2 v3 = ( center_pos + vec2(  radius, -radius ) ) / push_constants.sensor_size;
    const vec4 energy = vec4(
      light_pool[ global_uniforms.light ].energy.xyz *
      min( (push_constants.lens_radius*push_constants.lens_radius)/max(radius*radius,0.0000001), 10000.0 ) *
      //1.0/(1920*1080)
      0.05,
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
}

