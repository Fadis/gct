//
// Description : Array and textureless GLSL 2D simplex noise function.
//      Author : Ian McEwan, Ashima Arts.
//  Maintainer : stegu
//     Lastmod : 20110822 (ijm)
//     License : Copyright (C) 2011 Ashima Arts. All rights reserved.
//               Distributed under the MIT License. See LICENSE file.
//               https://github.com/ashima/webgl-noise
//               https://github.com/stegu/webgl-noise
// 
#ifndef GCT_SHADER_SIMPLEX_NOISE_H
#define GCT_SHADER_SIMPLEX_NOISE_H

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

float noise_simplex2d( vec2 v ) {
  const vec4 C = vec4(0.211324865405187f,  // (3.0-sqrt(3.0))/6.0
                      0.366025403784439f,  // 0.5*(sqrt(3.0)-1.0)
                     -0.577350269189626f,  // -1.0 + 2.0 * C.x
                      0.024390243902439f); // 1.0 / 41.0
// First corner
  vec2 i  = floor(v + dot( v, vec2( C.y, C.y ) ) );
  vec2 x0 = v -   i + dot( i, vec2( C.x, C.x ) );

// Other corners
  vec2 i1;
  //i1.x = step( x0.y, x0.x ); // x0.x > x0.y ? 1.0 : 0.0
  //i1.y = 1.0 - i1.x;
  i1 = ( x0.x > x0.y ) ? vec2( 1.0f, 0.0f ) : vec2( 0.0f, 1.0f );
  // x0 = x0 - 0.0 + 0.0 * C.xx ;
  // x1 = x0 - i1 + 1.0 * C.xx ;
  // x2 = x0 - 1.0 + 2.0 * C.xx ;
  vec4 x12 = vec4( x0.x, x0.y, x0.x, x0.y ) + vec4( C.x, C.x, C.z, C.z );
  x12.x -= i1.x;
  x12.y -= i1.y;

// Permutations
  i = noise_mod289(i); // Avoid truncation effects in permutation
  vec3 p = noise_permute(
    noise_permute(
      i.y + vec3( 0.0f, i1.y, 1.0f )
    ) +
    i.x + vec3( 0.0f, i1.x, 1.0f )
  );

  vec2 x12xy = vec2( x12.x, x12.y );
  vec2 x12zw = vec2( x12.z, x12.w );
  vec3 m = max( 0.5f - vec3( dot( x0, x0 ), dot( x12xy, x12xy ), dot( x12zw, x12zw ) ), 0.0f );
  m = m*m ;
  m = m*m ;

// Gradients: 41 points uniformly over a line, mapped onto a diamond.
// The ring size 17*17 = 289 is close to a multiple of 41 (41*7 = 287)

  vec3 x = 2.0f * fract( p * vec3( C.w, C.w, C.w ) ) - 1.0f;
  vec3 h = abs( x ) - 0.5f;
  vec3 ox = floor( x + 0.5f );
  vec3 a0 = x - ox;

// Normalise gradients implicitly by scaling m
// Approximation of: m *= inversesqrt( a0*a0 + h*h );
  m *= 1.79284291400159f - 0.85373472095314f * ( a0 * a0 + h * h );

// Compute final noise value at P
  vec3 g;
  g.x  = a0.x * x0.x  + h.x  * x0.y;
  g.y = a0.y * x12.x + h.y * x12.y;
  g.z = a0.z * x12.z + h.z * x12.w;
  return 130.0f * dot( m, g );
}


float noise_simplex2d_fbm5( vec2 v ) {
  float sum = 0.0f;
  float amp = 0.5f;
  for( unsigned int i = 0u; i != 5u; i++ ) {
    sum += amp * noise_simplex2d( v );
    v *= 2.0f;
    amp *= 0.5f;
  }
  return sum;
}

float noise_simplex2d_fbm10( vec2 v ) {
  float sum = 0.0f;
  float amp = 0.5f;
  for( unsigned int i = 0u; i != 10u; i++ ) {
    sum += amp * noise_simplex2d( v );
    v *= 2.0f;
    amp *= 0.5f;
  }
  return sum;
}

#endif

