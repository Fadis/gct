#ifndef GCT_SHADER_RANDOM_H
#define GCT_SHADER_RANDOM_H

float rand1( vec2 n ) {
  return fract( sin( dot( n.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

#endif

