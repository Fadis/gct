#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (triangles) in;
layout (triangle_strip,max_vertices=6) out;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec4 input_position[];
layout (location = 1) in vec4 input_scale[];

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec4 output_texcoord;

#include "push_constants.h"

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
  vec4 pos = gl_in[ 0 ].gl_Position;
  pos /= pos.w;

  // 頂点シェーダで求めた大きさの三角形を2枚生成する
  // スクリーン空間でオフセットを足しているので、三角形は常に視点の方を向く
  for( int i = 0; i < 3; i++ ) {
    vec4 o = offset[ i ] * input_scale[ 0 ];
    gl_Position = pos + o;
    output_position = input_position[ 0 ];
    output_texcoord = offset[ i ];
    EmitVertex();
  }
  EndPrimitive();
  for( int i = 3; i < 6; i++ ) {
    vec4 o = offset[ i ] * input_scale[ 0 ];
    gl_Position = pos + o;
    output_position = pos;
    output_texcoord = offset[ i ];
    EmitVertex();
  }
  EndPrimitive();
}

