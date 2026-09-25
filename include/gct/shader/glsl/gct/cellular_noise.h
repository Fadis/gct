// Cellular noise ("Worley noise") in 2D in GLSL.
// Copyright (c) Stefan Gustavson 2011-04-19. All rights reserved.
// This code is released under the conditions of the MIT license.
// See LICENSE file for details.
// https://github.com/stegu/webgl-noise

#ifndef GCT_SHADER_CELLULAR_NOISE_H
#define GCT_SHADER_CELLULAR_NOISE_H

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

vec3 noise_mod7( vec3 x ) {
  return x - floor( x * ( 1.0f / 7.0f ) ) * 7.0f;
}

// Cellular noise, returning F1 and F2 in a vec2.
// Standard 3x3 search window for good F1 and F2 values
vec2 noise_cellular( vec2 P ) {
  const float K = 0.142857142857f; // 1/7
  const float Ko = 0.428571428571f; // 3/7
  const float jitter = 1.0f; // Less gives more regular pattern
	vec2 Pi = noise_mod289( floor( P ) );
 	vec2 Pf = fract( P );
	vec3 oi = vec3( -1.0f, 0.0f, 1.0f );
	vec3 of = vec3( -0.5f, 0.5f, 1.5f );
	vec3 px = noise_permute( Pi.x + oi );
	vec3 p = noise_permute( px.x + Pi.y + oi ); // p11, p12, p13
	vec3 ox = fract( p * K ) - Ko;
	vec3 oy = noise_mod7( floor( p * K ) ) * K - Ko;
	vec3 dx = Pf.x + 0.5f + jitter * ox;
	vec3 dy = Pf.y - of + jitter * oy;
	vec3 d1 = dx * dx + dy * dy; // d11, d12 and d13, squared
	p = noise_permute( px.y + Pi.y + oi ); // p21, p22, p23
	ox = fract( p * K ) - Ko;
	oy = noise_mod7( floor( p * K ) ) * K - Ko;
	dx = Pf.x - 0.5f + jitter * ox;
	dy = Pf.y - of + jitter * oy;
	vec3 d2 = dx * dx + dy * dy; // d21, d22 and d23, squared
	p = noise_permute( px.z + Pi.y + oi ); // p31, p32, p33
	ox = fract( p * K ) - Ko;
	oy = noise_mod7( floor( p * K ) ) * K - Ko;
	dx = Pf.x - 1.5f + jitter * ox;
	dy = Pf.y - of + jitter * oy;
	vec3 d3 = dx * dx + dy * dy; // d31, d32 and d33, squared
	// Sort out the two smallest distances (F1, F2)
	vec3 d1a = min( d1, d2 );
	d2 = max( d1, d2 ); // Swap to keep candidates for F2
	d2 = min( d2, d3 ); // neither F1 nor F2 are now in d3
	d1 = min( d1a, d2 ); // F1 is now in d1
	d2 = max( d1a, d2 ); // Swap to keep candidates for F2
  vec2 d1xy = ( d1.x < d1.y ) ? vec2( d1.x, d1.y ) : vec2( d1.y, d1.x ); // Swap if smaller
  d1.x = d1xy.x;
  d1.y = d1xy.y;
  vec2 d1xz = ( d1.x < d1.z ) ? vec2( d1.x, d1.z ) : vec2( d1.z, d1.x ); // F1 is in d1.x
  d1.x = d1xz.x;
  d1.z = d1xz.y;
	vec2 d1yz = min( vec2( d1.y, d1.z ), vec2( d2.y, d2.z ) ); // F2 is now not in d2.yz
  d1.y = d1yz.x;
  d1.z = d1yz.y;
	d1.y = min( d1.y, d1.z ); // nor in  d1.z
	d1.y = min( d1.y, d2.x ); // F2 is in d1.y, we're done.
	return sqrt( vec2( d1.x, d1.y ) );
}

#endif

