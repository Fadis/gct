#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "scene_graph.h"
#include "global_uniforms.h"

layout (location = 0) in vec2 input_position;
layout (location = 0) out vec2 output_light0;
layout (location = 1) out vec2 output_light1;
layout (location = 2) out vec2 output_flare_direction;
layout (location = 3) flat out uint output_index;

out gl_PerVertex
{
    vec4 gl_Position;
};

// カメラ座標系での光源の位置とレンズの半径から追跡する必要がある2本の光のベクトルを求める
vec4 get_light_dir( vec3 light_pos, float lens_radius ) {
  const vec3 eye_dir = vec3( 0.0, 0.0, 1.0 );
  const vec2 lens_edge = normalize( light_pos.xy ) * lens_radius;
  float theta0 = light_pos.z != 0 ? max( min( length( light_pos.xy - lens_edge )/-light_pos.z, 1000.0 ), -1000.0 ) : 0.0;
  float theta1 = light_pos.z != 0 ? max( min( length( light_pos.xy + lens_edge )/-light_pos.z, 1000.0 ), -1000.0 ) : 0.0;
  return vec4( -lens_radius, theta0, lens_radius, theta1 );
}

void main() {
  const vec3 pos = light_pool[ global_uniforms.light ].world_position.xyz;
  const vec3 light_pos_in_camera =
    ( matrix_pool[ global_uniforms.camera_matrix ] * vec4( pos, 1.0 ) ).xyz;
  // レンズの端に届く光のrとθを求めてジオメトリシェーダに渡す
  const vec4 light = get_light_dir( light_pos_in_camera, push_constants.lens_radius );
  output_light0 = light.xy;
  output_light1 = light.zw;
  // 何番目の経路を使うかを指定する
  output_index = gl_InstanceIndex;
  // スクリーン空間でレンズフレアが現れる直線を表すベクトル
  output_flare_direction = length( light_pos_in_camera.xy ) > 0.01 ? normalize( light_pos_in_camera.xy ) : vec2( 0.0, 0.0 );
  // スクリーン空間で光源がどこに位置するか
  gl_Position =
    matrix_pool[ global_uniforms.projection_matrix ] *
    matrix_pool[ global_uniforms.camera_matrix ] * vec4( pos, 1.0 );
}

