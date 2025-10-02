#ifndef GCT_SHADER_RANDOM_H
#define GCT_SHADER_RANDOM_H

float rand1( vec2 n ) {
  return fract( sin( dot( n.xy, vec2( 12.9898, 78.233 ) ) ) * 43758.5453 );
}

#if 0
uint tausworthe( inout uint z, uint s1, uint s2, uint s3, uint m ) {
  uint b = ((( z << s1 ) ^ z ) >> s2 );
  return ((( z & m ) << s3 ) ^ b );
}

uint lcg( inout uint z, uint a, uint c ) {
  return ( a * z + c );
}

uvec4 init_hybrid_taus( uvec4 tid ) {
  return tid + uvec4( 1613128450, 1760096502, 255990474, 1986814468 );
}

uint hybrid_taus_uint( inout uvec4 state ) {
  return
    tausworthe( state.x, 13, 19, 12, 4294967294UL ) ^
    tausworthe( state.y, 2, 25, 4, 4294967288UL ) ^
    tausworthe( state.z, 3, 11, 17, 4294967280UL ) ^
    lcg( state.w, 1664525, 1013904223UL );
}

float hybrid_taus_float( inout uvec4 state ) {
  return 2.3283064365387e-10 * hybrid_taus_uint( state );
}
#endif

#endif

