#ifndef GCT_SHADER_GAMMA_H
#define GCT_SHADER_GAMMA_H

vec3 gamma( vec3 v ) {
  return min( max( v / (v + 0.155 ) * 1.019, vec3( 0, 0, 0 ) ), vec3( 1, 1, 1 ) );
}

#endif

