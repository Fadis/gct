#ifndef GCT_SHADER_QUATERNION_H
#define GCT_SHADER_QUATERNION_H

mat3 quaternion_to_matrix( vec4 q ) {
  const float qxx = q.x * q.x;
  const float qyy = q.y * q.y;
  const float qzz = q.z * q.z;
  const float qxz = q.x * q.z;
  const float qxy = q.x * q.y;
  const float qyz = q.y * q.z;
  const float qwx = q.w * q.x;
  const float qwy = q.w * q.y;
  const float qwz = q.w * q.z;

  mat3 result;
  result[ 0 ][ 0 ] = 1.0f - 2.0f * ( qyy + qzz );
	result[ 0 ][ 1 ] = 2.0f * ( qxy + qwz );
	result[ 0 ][ 2 ] = 2.0f * ( qxz - qwy );

	result[ 1 ][ 0 ] = 2.0f * ( qxy - qwz );
	result[ 1 ][ 1 ] = 1.0f - 2.0f * ( qxx + qzz );
	result[ 1 ][ 2 ] = 2.0f * ( qyz + qwx );

	result[ 2 ][ 0 ] = 2.0f * ( qxz + qwy );
	result[ 2 ][ 1 ] = 2.0f * ( qyz - qwx );
	result[ 2 ][ 2 ] = 1.0f - 2.0f * ( qxx + qyy );
  return result;
}

vec4 matrix_to_quaternion( mat3 m ) {
	const float fourXSquaredMinus1 = m[ 0 ][ 0 ] - m[ 1 ][ 1 ] - m[ 2 ][ 2 ];
	const float fourYSquaredMinus1 = m[ 1 ][ 1 ] - m[ 0 ][ 0 ] - m[ 2 ][ 2 ];
	const float fourZSquaredMinus1 = m[ 2 ][ 2 ] - m[ 0 ][ 0 ] - m[ 1 ][ 1 ];
	const float fourWSquaredMinus1 = m[ 0 ][ 0 ] + m[ 1 ][ 1 ] + m[ 2 ][ 2 ];

  int biggestIndex = 0;
  float fourBiggestSquaredMinus1 = fourWSquaredMinus1;
  if( fourXSquaredMinus1 > fourBiggestSquaredMinus1 ) {
    fourBiggestSquaredMinus1 = fourXSquaredMinus1;
		biggestIndex = 1;
  }
	if( fourYSquaredMinus1 > fourBiggestSquaredMinus1 ) {
		fourBiggestSquaredMinus1 = fourYSquaredMinus1;
		biggestIndex = 2;
	}
	if( fourZSquaredMinus1 > fourBiggestSquaredMinus1 ) {
		fourBiggestSquaredMinus1 = fourZSquaredMinus1;
		biggestIndex = 3;
	}

  const float biggestVal = sqrt( fourBiggestSquaredMinus1 + 1.0 ) * 0.5;
	const float mult = 0.25 / biggestVal;

  return (
    ( biggestIndex == 0 ) ?
    vec4(
      ( m[ 1 ][ 2 ] - m[ 2 ][ 1 ] ) * mult, ( m[ 2 ][ 0 ] - m[ 0 ][ 2 ] ) * mult, ( m[ 0 ][ 1 ] - m[ 1 ][ 0 ] ) * mult, biggestVal
    ) : (
      ( biggestIndex == 1 ) ?
      vec4(
        biggestVal, ( m[ 0 ][ 1 ] + m[ 1 ][ 0 ] ) * mult, ( m[ 2 ][ 0 ] + m[ 0 ][ 2 ] ) * mult, ( m[ 1 ][ 2 ] - m[ 2 ][ 1 ] ) * mult
      ) : (
        ( biggestIndex == 2 ) ?
        vec4(
          ( m[ 0 ][ 1 ] + m[ 1 ][ 0 ] ) * mult, biggestVal, ( m[ 1 ][ 2 ] + m[ 2 ][ 1 ] ) * mult, ( m[ 2 ][ 0 ] - m[ 0 ][ 2 ] ) * mult
        ) :
        vec4(
          ( m[ 2 ][ 0 ] + m[ 0 ][ 2 ] ) * mult, ( m[ 1 ][ 2 ] + m[ 2 ][ 1 ] ) * mult, biggestVal, (m[ 0 ][ 1 ] - m[ 1 ][ 0 ] ) * mult
        )
      )
    )
  );
}

vec4 quaternion_quaternion_mult( vec4 q1, vec4 q2 ) {
  return vec4(
    q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
    q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
  );
}

vec3 quaternion_vector_mult( vec4 q, vec3 v ) {
  vec3 quat_vector = vec3( q );
  vec3 uv = cross( quat_vector, v );
  vec3 uuv = cross( quat_vector, uv );
  return v + ( ( uv * q.w ) + uuv ) * 2.0f;
}

vec4 quaternion_conjugate( vec4 q ) {
  return vec4( -q.x, -q.y, -q.z, q.w );
}

vec4 quaternion_inverse( vec4 q ) {
  return quaternion_conjugate( q ) / dot( q, q );
}

vec4 quaternion_mix( vec4 x, vec4 y, float a ) {
  const float cos_theta = dot( x, y );
  const float epsilon = 0.0001;
  if( cos_theta > 1.0 - epsilon ) {
    return mix( x, y, a );
  }
  float angle = acos( cos_theta );
  return ( float( sin( ( 1.0f - a ) ) * angle ) * x + float( sin( a * angle ) ) * y ) / float( sin( angle ) );
}

vec4 quaternion_normalize( vec4 q ) {
  const float len = length( q );
  const float one_over_len = 1.0f / len;
  return q * one_over_len;
}

vec4 quaternion_slerp( vec4 x, vec4 y, float a ) {
  vec4 z = y;
  float cos_theta = dot( x, y );
  if( cos_theta < 0.0 ) {
    z = -y;
    cos_theta = -cos_theta;
  }
  float angle = acos( cos_theta );
  return ( float( sin( ( 1.0 - a ) * angle ) ) * x + float( sin( a * angle ) ) * z ) / float( sin( angle ) );
}

#endif

