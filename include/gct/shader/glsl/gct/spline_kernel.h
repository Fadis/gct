#ifndef GCT_SHADER_SPLINE_KERNEL_H
#define GCT_SHADER_SPLINE_KERNEL_H

#include <gct/constants.h>

float spline_kernel( vec3 r, float h ) {
  const float rd = length( r );
  const float rh = rd/h;
  float rh2 = 2.0 - rh;
  float alpha = 1.0 / ( pi * h * h * h );
  return alpha * (
    ( 0.0 <= rh && rh <= 1.0 ) ?
    ( 1.0 - 3.0 / 2.0 * rh * rh + 3.0 / 4.0 * rh * rh * rh ) :
    (
      ( 1.0 <= rh && rh <= 2.0 ) ?
      ( 1.0 / 4.0 * rh2 * rh2 * rh2 ) :
      0.0
    )
  );
}

vec3 spline_kernel_gradient( vec3 r, float h ) {
  const float rd = length( r );
  const float rh = rd/h;
  const float rh2 = ( 2.0 - rh );
  const float alpha = 9.0 / ( 4.0 * pi * h * h * h * h * h );
  return alpha * (
    ( 0.0 <= rh && rh <= 1.0 ) ?
    ( float( rh - 4.0 / 3.0 ) * r ) :
    (
      ( 1.0 <= rh && rh <= 2.0 ) ?
      ( float( -1.0 / 3.0 * rh2 * rh2 * h / rd ) * r ) :
      vec3( 0.0, 0.0, 0.0 )
    )
  );
}

float spline_kernel_laplacian( vec3 r, float h ) {
  const float rd = length( r );
  const float rh = rd/h;
  const float rh2 = ( 2.0 - rh );
  const float alpha = 9.0 / ( 4.0 * pi * h * h * h * h * h );
  const float d = 3.0;
  return alpha / 3.0 * (
    ( 0.0 <= rh && rh <= 1.0 ) ?
    ( 3.0 * ( d + 1.0 ) * rh - 4.0 * d ) :
    (
      ( 1.0 <= rh && rh <= 2.0 ) ?
      ( ( 1.0 - d ) * rh2 * rh2 * ( h / rd ) + 2.0 * rh2 ) :
      0.0
    )
  );
}


#endif

