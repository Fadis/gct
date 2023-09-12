
struct SHCoef {
  vec4 coef[ 9 ];
};


SHCoef zero_coefficients() {
  SHCoef temp;
  for( uint i = 0; i != 9; i++ ) {
    temp.coef[ i ] = vec4( 0, 0, 0, 0 );
  }
  return temp;
}

float[9] get_basis_at( vec3 normal ) {
  return float[](
    0.282095,
    0.488603 * normal.y,
    0.488603 * normal.z,
    0.488603 * normal.x,
    1.092548 * normal.x * normal.y,
    1.092548 * normal.y * normal.z,
    0.315392 * ( 3 * normal.z * normal.z - 1 ),
    1.092548 * normal.x * normal.z,
    0.546274 * ( normal.x * normal.x - normal.y * normal.y )
  );
}

SHCoef get_partial_coefficients( vec3 normal, vec4 value ) {
  float[9] basis = get_basis_at( normal );
  SHCoef result;
  for( uint i = 0; i != 9; i++ ) {
    result.coef[ i ] = value * basis[ i ];
  }
  return result;
}

vec4 get_irradiance_at( vec3 normal, SHCoef sh ) {
  // band 0
  vec4 sum = sh.coef[ 0 ] * 0.886227;
  // band 1
  sum += sh.coef[ 1 ] * 2.0 * 0.511664 * normal.y;
  sum += sh.coef[ 2 ] * 2.0 * 0.511664 * normal.z;
  sum += sh.coef[ 3 ] * 2.0 * 0.511664 * normal.x;
  // band 2
  sum += sh.coef[ 4 ] * 2.0 * 0.429043 * normal.x * normal.y;
  sum += sh.coef[ 5 ] * 2.0 * 0.429043 * normal.y * normal.z;
  sum += sh.coef[ 6 ] * ( 0.743125 * normal.z * normal.z - 0.247708 );
  sum += sh.coef[ 7 ] * 2.0 * 0.429043 * normal.x * normal.z;
  sum += sh.coef[ 8 ] * 0.429043 * ( normal.x * normal.x - normal.y * normal.y );
  return sum;
}

