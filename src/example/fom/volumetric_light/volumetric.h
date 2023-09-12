
float henyey_greenstein(
  float g,
  float theta
) {
  float p = 1 + g * g - 2 * g * cos( theta );
  return 1.0 / ( 4.0 * pi ) * ( ( 1.0 - g * g ) / sqrt( p * p * p ) );
}

vec3 henyey_greenstein_schlick(
  vec3 g,
  float theta
) {
  const vec3 k = 1.55 * g - 0.55 * g * g * g;
  vec3 p = 1 + k * cos( theta );
  return ( 1.0 - k * k )/( 4.0 * pi * p * p );
}

float henyey_greenstein_schlick(
  float g,
  float theta
) {
  const float k = 1.55 * g - 0.55 * g * g * g;
  float p = 1 + k * cos( theta );
  return ( 1.0 - k * k )/( 4.0 * pi * p * p );
}

vec3 beer_lambert( vec3 sigma, float distance ) {
  return exp( -( sigma * distance ) );
}

