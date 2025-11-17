#ifndef GCT_SHADER_TRS_H
#define GCT_SHADER_TRS_H

#include <gct/quaternion.h>

mat4 matrix_to_trs( mat4 m ) {
  vec3 t = vec3( m[ 3 ] );
  const bool inverted = dot( cross( vec3( m[ 0 ] ), vec3( m[ 1 ] ) ), vec3( m[ 2 ] ) ) < 0.0;
  vec3 s = vec3(
    length( vec3( m[ 0 ] ) ) * ( inverted ? -1 : 1 ),
    length( vec3( m[ 1 ] ) ) * ( inverted ? -1 : 1 ),
    length( vec3( m[ 2 ] ) ) * ( inverted ? -1 : 1 )
  );
  mat3 ms = mat3( m );
  ms[ 0 ] /= s[ 0 ];
  ms[ 1 ] /= s[ 1 ];
  ms[ 2 ] /= s[ 2 ];
  vec4 r = matrix_to_quaternion( ms );
  return mat4(
    vec4( t, 1.0 ),
    r,
    vec4( s, 1.0 ),
    vec4( 0, 0, 0, 0 )
  );
}

mat4 trs_to_matrix( mat4 trs ) {
  mat4 m = mat4( quaternion_to_matrix( trs[ 1 ] ) );
  m[ 3 ] = trs[ 0 ];
  m[ 0 ] *= trs[ 2 ][ 0 ]; 
  m[ 1 ] *= trs[ 2 ][ 1 ];
  m[ 2 ] *= trs[ 2 ][ 2 ];
  return m;
}

#endif

