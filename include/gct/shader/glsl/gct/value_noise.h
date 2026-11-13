#ifndef GCT_SHADER_VALUE_NOISE_H
#define GCT_SHADER_VALUE_NOISE_H

#ifdef __cplusplus
#include <gct/shader/glsl/gct/perlin_noise_common.h>
#include <gct/shader/glsl/gct/random.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;
#else
#include <gct/perlin_noise_commo.h>
#include <gct/random.h>
#endif

// Classic Perlin noise
float noise_value2d( vec2 P ) {
  const vec4 pxyxy = vec4( P.x, P.y, P.x, P.y );
  // Pを内包する単位矩形領域の左下の頂点の座標
  vec2 Pi = floor(P);
  // 単位矩形領域の左下の頂点からPまでのオフセット
  const vec2 Pf = fract(P);
  // 単位矩形領域の頂点の座標を289で割った余りにする
  // 精度不足対策
  Pi = noise_mod289( Pi ); // To avoid truncation effects in permutation
  // 4つの頂点での値を乱数で決める
  vec4 value = vec4(
    rand1( Pi ),
    rand1( Pi + vec2( 1, 0 ) ),
    rand1( Pi + vec2( 0, 1 ) ),
    rand1( Pi + vec2( 1, 1 ) )
  );
  // 線形補間
  return mix(
    mix( value.x, value.y, Pf.x ),
    mix( value.z, value.w, Pf.x ),
    Pf.y
  );
}

// Classic Perlin noise
float noise_value2d_point( vec2 P ) {
  const vec4 pxyxy = vec4( P.x, P.y, P.x, P.y );
  // Pを内包する単位矩形領域の左下の頂点の座標
  vec2 Pi = floor(P);
  // 単位矩形領域の左下の頂点からPまでのオフセット
  const vec2 Pf = fract(P);
  // 単位矩形領域の頂点の座標を289で割った余りにする
  // 精度不足対策
  Pi = noise_mod289( Pi ); // To avoid truncation effects in permutation
  // 4つの頂点での値を乱数で決める
  vec4 value = vec4(
    rand1( Pi ),
    rand1( Pi + vec2( 1, 0 ) ),
    rand1( Pi + vec2( 0, 1 ) ),
    rand1( Pi + vec2( 1, 1 ) )
  );

  float v = 0.0f;
  if( length( Pf ) < 0.03 ) v = value.x;
  else if( length( Pf - vec2( 1, 0 ) ) < 0.03 ) v = value.y;
  else if( length( Pf - vec2( 0, 1 ) ) < 0.03 ) v = value.z;
  else if( length( Pf - vec2( 1, 1 ) ) < 0.03 ) v = value.w;
  return v;
}


float noise_value2d_fbm5( vec2 v ) {
  float sum = 0.0f;
  float amp = 0.5f;
  for( unsigned int i = 0u; i != 5u; i++ ) {
    sum += amp * noise_value2d( v );
    v *= 2.0f;
    amp *= 0.5f;
  }
  return sum;
}

#endif


