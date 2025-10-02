#version 460
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable
#extension GL_EXT_nonuniform_qualifier : enable
#extension GL_EXT_shader_image_load_formatted : enable

#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

#define GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>

layout(push_constant) uniform PushConstants {
  vec2 sensor_size;
  uint texture_id;
  float lens_radius;
} push_constants;

layout (points) in;
layout (triangle_strip,max_vertices=18) out;

const uint flare_matrix_count = 36;
// 事前に求めた36種類のレンズフレアの経路の行列
const mat2 flare_matrices[36]=mat2[](
#include <gct/lens_flare/prtlf_flare.inc>
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
  ivec2 screen_pos = ivec2( imageSize( image_pool_2d[ global_uniforms.gbuffer ] ).xy * ( light_pos_in_screen.xy * vec2( 0.5,  0.5 ) + 0.5 ) );
  kplus_iter iter = kplus_begin(
    kplus_image( global_uniforms.gbuffer, global_uniforms.depth ),
    screen_pos
  );
  iter = kplus_nearest( iter );
  const float depth = 
    !kplus_is_end( iter ) ?
    kplus_get_depth( iter ):
    2.0;
  if(
    light_pos_in_screen.x < -1.0 || light_pos_in_screen.x > 1.0 ||
    light_pos_in_screen.y < -1.0 || light_pos_in_screen.y > 1.0 ||
   light_pos_in_screen.z < -1.0 || light_pos_in_screen.z > depth
  ) return;
  const uint i = output_index[ 0 ];
  {
    const float r0 = ( flare_matrices[ i ] * input_light0[ 0 ] ).x;
    const float r1 = ( flare_matrices[ i ] * input_light1[ 0 ] ).x;
    // レンズフレアの中心座標と半径を求める
    const float center = ( r0 + r1 )*0.5;
    const float radius = abs( r0 - r1 )*0.5;
    // レンズフレアが現れる範囲にビルボードを作る
    const vec2 center_pos = input_flare_direction[ 0 ] * center;
    const vec2 v0 = ( center_pos + vec2( -radius,  radius ) ) / push_constants.sensor_size;
    const vec2 v1 = ( center_pos + vec2( -radius, -radius ) ) / push_constants.sensor_size;
    const vec2 v2 = ( center_pos + vec2(  radius,  radius ) ) / push_constants.sensor_size;
    const vec2 v3 = ( center_pos + vec2(  radius, -radius ) ) / push_constants.sensor_size;
    // レンズフレアでイメージセンサーの各ピクセルに届く光のエネルギーを求める
    // レンズを通る光のエネルギーは一定なので、それが広範囲に散らばるほど1ピクセルあたりのエネルギーは小さくなる
    // 1回の反射でエネルギーが5%になる為, 2回反射した光のエネルギーは元の0.05*0.05倍になる
    const vec4 energy = vec4(
      light_pool[ global_uniforms.light ].energy.xyz *
      min( (push_constants.lens_radius*push_constants.lens_radius)/max(radius*radius,0.0000001), 10000.0 ) *
      0.05*0.05,
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

