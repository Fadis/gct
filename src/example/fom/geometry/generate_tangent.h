#ifndef GCT_SHADER_GET_TANGENT_H
#define GCT_SHADER_GET_TANGENT_H

vec3 generate_tangent( vec3 normal ) {
  if( abs( dot( vec3( 0.0, 1.0, 0.0 ), normal ) ) > 0.7071067811865476 ) { // cos( pi/4 )
    return cross( vec3( 0.0, 0.0, 1.0 ), normal );
  }
  else {
    return cross( vec3( 0.0, 1.0, 0.0 ), normal );
  }
}

#endif

