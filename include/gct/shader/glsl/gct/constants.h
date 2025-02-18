#ifndef GCT_SHADER_CONSTANTS_H
#define GCT_SHADER_CONSTANTS_H

const float pi = 3.141592653589793;
const float sqrt2 = 1.4142135623730951;
const vec3 rgb2y = vec3( 0.299, 0.587, 0.114 );
const mat3 bt709_to_xyz = mat3(
  0.4124079, 0.2126478, 0.0193316,
  0.3575896, 0.7151791, 0.1191965,
  0.1804326, 0.0721730, 0.9502783
);

#endif

