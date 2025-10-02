#ifndef GCT_SHADER_LENS_FLARE_PRTLF_H
#define GCT_SHADER_LENS_FLARE_PRTLF_H

// カメラ座標系での光源の位置とレンズの半径から追跡する必要がある2本の光のベクトルを求める
vec4 prtlf_get_light_dir( vec3 light_pos, float lens_radius ) {
  const vec3 eye_dir = vec3( 0.0, 0.0, 1.0 );
  const vec2 lens_edge = normalize( light_pos.xy ) * lens_radius;
  float theta0 = light_pos.z != 0 ? max( min( length( light_pos.xy - lens_edge )/-light_pos.z, 1000.0 ), -1000.0 ) : 0.0;
  float theta1 = light_pos.z != 0 ? max( min( length( light_pos.xy + lens_edge )/-light_pos.z, 1000.0 ), -1000.0 ) : 0.0;
  return vec4( -lens_radius, theta0, lens_radius, theta1 );
}

#endif

