#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (triangles) in;
layout (triangle_strip,max_vertices=36) out;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) out vec4 output_texcoord;
layout (location = 1) out float output_linear_z;

#include "push_constants.h"

float get_linear_z( float d, float z_near, float z_far ) {
  return ( ( 2.0 * z_near * z_far / ( z_far + z_near - d * ( z_far - z_near ) ) ) - z_near ) / abs( z_far - z_near );
}

layout(std430, binding = 1) buffer OutputMatrices {
  mat4 camera_projection[6];
  mat4 camera[6];
} matrices;

layout(push_constant) uniform PushConstants {
  float near;
  float far;
  float aspect;
  float point_size;
  uint split_count;
  float color;
} push_constants;

vec4 offset[6]=vec4[](
  vec4( -1.0, -1.0, 0.0, 0.0 ),
  vec4( 1.0, -1.0, 0.0, 0.0 ),
  vec4( -1.0, 1.0, 0.0, 0.0 ),
  vec4( -1.0, 1.0, 0.0, 0.0 ),
  vec4( 1.0, -1.0, 0.0, 0.0 ),
  vec4( 1.0, 1.0, 0.0, 0.0 )
);

void main() {
  gl_PrimitiveID = gl_PrimitiveIDIn;
  const vec4 pos = gl_in[ 0 ].gl_Position;

  // キューブマップにビルボードを描くので, 6つの面それぞれに対して頂点を吐く
  for( int i = 0; i < 6; i++ ) {
    const vec4 camera_space_pos = matrices.camera[ i ] * pos;
    // 投影行列に設定された値に基づいて遠くの点ほど小さいビルボードが描かれるようにする
    const float scale_factor = camera_space_pos.z * dynamic_uniforms.projection_matrix[ 3 ][ 2 ];
    // アスペクト比で割って画面上でビルボードが正方形になるようにする
    const vec4 scale = vec4( push_constants.aspect/scale_factor, 1.0/scale_factor, 0.0, 1.0 );
    
    vec4 screen_pos = matrices.camera_projection[ i ] * pos;
    screen_pos /= screen_pos.w;
    // 線形な深度を求める
    const float linear_z = get_linear_z( screen_pos.z, push_constants.near, push_constants.far );
    
    // 頂点シェーダで求めた大きさの三角形を2枚生成する
    // スクリーン空間でオフセットを足しているので、三角形は常に視点の方を向く
    for( int j = 0; j < 3; j++ ) {
      const vec4 o = offset[ j ] * push_constants.point_size * scale;
      gl_Layer = int( i );
      gl_Position = screen_pos + o;
      output_texcoord = offset[ j ];
      output_linear_z = linear_z;
      EmitVertex();
    }
    EndPrimitive();
    for( int j = 3; j < 6; j++ ) {
      const vec4 o = offset[ j ] * push_constants.point_size * scale;
      gl_Layer = int( i );
      gl_Position = screen_pos + o;
      output_texcoord = offset[ j ];
      output_linear_z = linear_z;
      EmitVertex();
    }
    EndPrimitive();
  }
}

