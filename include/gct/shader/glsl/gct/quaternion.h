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
  result[ 0 ][ 0 ] = 1.0 - 2.0 * ( qyy + qzz );
	result[ 0 ][ 1 ] = 2.0 * ( qxy + qwz );
	result[ 0 ][ 2 ] = 2.0 * ( qxz - qwy );

	result[ 1 ][ 0 ] = 2.0 * ( qxy - qwz );
	result[ 1 ][ 1 ] = 1.0 - 2.0 * ( qxx + qzz );
	result[ 1 ][ 2 ] = 2.0 * ( qyz + qwx );

	result[ 2 ][ 0 ] = 2.0 * ( qxz + qwy );
	result[ 2 ][ 1 ] = 2.0 * ( qyz - qwx );
	result[ 2 ][ 2 ] = 1.0 - 2.0 * ( qxx + qyy );
  return result;
}

vec4 matrix_to_quaterion( mat3 m ) {
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

vec4 quaternion_cross( vec4 q1, vec4 q2 ) {
  return vec4(
    q1.w * q2.x + q1.x * q2.w + q1.y * q2.z - q1.z * q2.y,
		q1.w * q2.y + q1.y * q2.w + q1.z * q2.x - q1.x * q2.z,
		q1.w * q2.z + q1.z * q2.w + q1.x * q2.y - q1.y * q2.x,
    q1.w * q2.w - q1.x * q2.x - q1.y * q2.y - q1.z * q2.z
  );
}

#endif

