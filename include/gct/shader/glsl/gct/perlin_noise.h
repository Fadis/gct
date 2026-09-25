//
// GLSL textureless classic 2D noise "cnoise",
// with an RSL-style periodic variant "pnoise".
// Author:  Stefan Gustavson (stefan.gustavson@gmail.com)
// Version: 2024-11-07
//
// Many thanks to Ian McEwan of Ashima Arts for the
// ideas for permutation and gradient selection.
//
// Copyright (c) 2011 Stefan Gustavson. All rights reserved.
// Distributed under the MIT license. See LICENSE file.
// https://github.com/stegu/webgl-noise
//

#ifndef GCT_SHADER_PERLIN_NOISE_H
#define GCT_SHADER_PERLIN_NOISE_H

#ifdef __cplusplus
#include <gct/shader/glsl/gct/perlin_noise_common.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
using namespace glm;
#else
#include <gct/perlin_noise_commo.h>
#endif

// Classic Perlin noise
float noise_perlin2d( vec2 P ) {
  const vec4 pxyxy = vec4( P.x, P.y, P.x, P.y );
  vec4 Pi = floor(pxyxy) + vec4( 0.0, 0.0, 1.0, 1.0 );
  const vec4 Pf = fract(pxyxy) - vec4( 0.0, 0.0, 1.0, 1.0 );
  Pi = noise_mod289( Pi ); // To avoid truncation effects in permutation
  vec4 ix = vec4( Pi.x, Pi.z, Pi.x, Pi.z );
  vec4 iy = vec4( Pi.y, Pi.y, Pi.w, Pi.w );
  vec4 fx = vec4( Pf.x, Pf.z, Pf.x, Pf.z );
  vec4 fy = vec4( Pf.y, Pf.y, Pf.w, Pf.w );

  vec4 i = noise_permute( noise_permute( ix ) + iy );

  vec4 gx = fract( i * ( 1.0f / 41.0f ) ) * 2.0f - 1.0f;
  vec4 gy = abs( gx ) - 0.5f ;
  vec4 tx = floor( gx + 0.5f );
  gx = gx - tx;

  vec2 g00 = vec2( gx.x, gy.x );
  vec2 g10 = vec2( gx.y, gy.y );
  vec2 g01 = vec2( gx.z, gy.z );
  vec2 g11 = vec2( gx.w, gy.w );

  vec4 norm = noise_taylor_inv_sqrt(
    vec4( dot( g00, g00 ), dot( g01, g01 ), dot( g10, g10 ), dot( g11, g11 ) )
  );

  float n00 = norm.x * dot( g00, vec2( fx.x, fy.x ) );
  float n01 = norm.y * dot( g01, vec2( fx.z, fy.z ) );
  float n10 = norm.z * dot( g10, vec2( fx.y, fy.y ) );
  float n11 = norm.w * dot( g11, vec2( fx.w, fy.w ) );

  vec2 fade_xy = noise_fade( vec2( Pf.x, Pf.y ) );
  vec2 n_x = mix( vec2( n00, n01 ), vec2( n10, n11 ), fade_xy.x );
  float n_xy = mix( n_x.x, n_x.y, fade_xy.y );
  return 2.3f * n_xy;
}

// Classic Perlin noise, periodic variant
float noise_perlin2d_periodic( vec2 P, vec2 rep ) {
  const vec4 pxyxy = vec4( P.x, P.y, P.x, P.y );
  vec4 Pi = floor(pxyxy) + vec4( 0.0, 0.0, 1.0, 1.0 );
  const vec4 Pf = fract(pxyxy) - vec4( 0.0, 0.0, 1.0, 1.0 );
  Pi = mod( Pi, vec4( rep.x, rep.y, rep.x, rep.y ) ); // To create noise with explicit period
  Pi = noise_mod289( Pi ); // To avoid truncation effects in permutation
  vec4 ix = vec4( Pi.x, Pi.z, Pi.x, Pi.z );
  vec4 iy = vec4( Pi.y, Pi.y, Pi.w, Pi.w );
  vec4 fx = vec4( Pf.x, Pf.z, Pf.x, Pf.z );
  vec4 fy = vec4( Pf.y, Pf.y, Pf.w, Pf.w );

  vec4 i = noise_permute( noise_permute( ix ) + iy );

  vec4 gx = fract( i * ( 1.0f / 41.0f ) ) * 2.0f - 1.0f;
  vec4 gy = abs( gx ) - 0.5f ;
  vec4 tx = floor( gx + 0.5f );
  gx = gx - tx;

  vec2 g00 = vec2(gx.x,gy.x);
  vec2 g10 = vec2(gx.y,gy.y);
  vec2 g01 = vec2(gx.z,gy.z);
  vec2 g11 = vec2(gx.w,gy.w);

  vec4 norm = noise_taylor_inv_sqrt(
    vec4( dot( g00, g00 ), dot( g01, g01 ), dot( g10, g10 ), dot( g11, g11 ) )
  );

  float n00 = norm.x * dot( g00, vec2( fx.x, fy.x ) );
  float n01 = norm.y * dot( g01, vec2( fx.z, fy.z ) );
  float n10 = norm.z * dot( g10, vec2( fx.y, fy.y ) );
  float n11 = norm.w * dot( g11, vec2( fx.w, fy.w ) );
  
  vec2 fade_xy = noise_fade( vec2( Pf.x, Pf.y ) );
  vec2 n_x = mix( vec2( n00, n01 ), vec2( n10, n11 ), fade_xy.x );
  float n_xy = mix( n_x.x, n_x.y, fade_xy.y );
  return 2.3f * n_xy;
}

float noise_perlin2d_fbm5( vec2 v ) {
  float sum = 0.0f;
  float amp = 0.5f;
  for( unsigned int i = 0u; i != 5u; i++ ) {
    sum += amp * noise_perlin2d( v );
    v *= 2.0f;
    amp *= 0.5f;
  }
  return sum;
}

#endif

