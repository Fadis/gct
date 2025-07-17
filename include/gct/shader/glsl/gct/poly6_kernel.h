#include <gct/constants.h>

float poly6_kernel( vec3 r, float h ) {
  const float rd = length( r );
  const float alpha = 315.0 / ( 64.0 * pi * pow( h, 9 ) );
  return
    ( rd < h ) ?
    ( alpha * pow( h * h - rd * rd, 3 ) ) :
    0.0;
}

vec3 poly6_kernel_gradient( vec3 r, float h ) {
  const float rd = length( r );
  const float alpha = 315.0 / ( 64.0 * pi * pow( h, 9 ) );
  return
    ( rd < h ) ?
    ( float( -6 * alpha * pow( h * h - rd * rd, 2 ) ) * r ) :
    vec3( 0.0, 0.0, 0.0 );
}


