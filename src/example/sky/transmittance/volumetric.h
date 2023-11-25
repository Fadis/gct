
float rayleigh(
  float g,
  float theta
) {
  const float cost = cos( theta );
  return g * ( 3.0 * ( 1.0 + cost*cost ) / ( 16.0 * pi ) );
}

vec3 rayleigh(
  vec3 g,
  float theta
) {
  const float cost = cos( theta );
  return g * ( 3.0 * ( 1.0 + cost*cost ) / ( 16.0 * pi ) );
}

float henyey_greenstein(
  float g,
  float theta
) {
  const float p = 1 + g * g - 2 * g * cos( theta );
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

vec3 beer_lambert( vec3 sigma, float distance ) {
  return exp( -( sigma * distance ) );
}

vec3 beer_lambert( vec3 sigma, float density, float distance ) {
  return exp( -( sigma * density * distance ) );
}

