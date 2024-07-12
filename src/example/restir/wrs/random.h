#ifndef GCT_SHADER_RANDOM_H
#define GCT_SHADER_RANDOM_H

float rand1( vec2 n ) {
  return fract( sin( dot( n.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

vec2 rand1_vec2( vec2 n ) {
  vec2 v;
  v.x = rand1( n );
  n.x *= v.x * 131.91;
  n.y *= v.x * 67.49;
  v.y = rand1( n );
  return v;
}

#endif

