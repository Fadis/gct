#ifndef GCT_VOLUMETRIC
#define GCT_VOLUMETRIC

#include <gct/constants.h>

float rayleigh(
  float theta
) {
  const float cost = cos( theta );
  return 3.0 * ( 1.0 + cost*cost ) / ( 16.0 * pi );
}

float henyey_greenstein(
  float g,
  float theta
) {
  const float p = 1 + g * g - 2 * g * cos( theta );
  return 1.0 / ( 4.0 * pi ) * ( ( 1.0 - g * g ) / sqrt( p * p * p ) );
}

float henyey_greenstein_schlick(
  float g,
  float theta
) {
  const float k = 1.55 * g - 0.55 * g * g * g;
  float p = 1 + k * cos( theta );
  return ( 1.0 - k * k )/( 4.0 * pi * p * p );
}

float henyey_greenstein_schlick2(
  float g,
  float theta
) {
  const float s = 3.0 / ( 8.0 * pi );
  const float ct = cos( theta );
  const float k = 1.0 + g * g - 2.0 * g * ct;
  float p = (1-g*g)*(1+ct*ct)/((2+g*g)*sqrt(k*k*k));
  return s * p;
}


vec3 beer_lambert( vec3 sigma, float distance ) {
  return exp( -( sigma * distance ) );
}

vec3 beer_lambert( vec3 sigma, float density, float distance ) {
  return exp( -( sigma * density * distance ) );
}

#endif

