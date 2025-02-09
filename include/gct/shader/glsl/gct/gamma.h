#ifndef GCT_SHADER_GAMMA_H
#define GCT_SHADER_GAMMA_H

const mat3 xyz_to_bt709 = mat3(
  3.2408357,  -0.9692294, 0.0556449,
  -1.5373195, 1.8759400,  -0.2040314,
  -0.4985901, 0.0415544,  1.0572538
);

vec3 gamma( vec3 v ) {
  return min( max( v / (v + 0.155 ) * 1.019, vec3( 0, 0, 0 ) ), vec3( 1, 1, 1 ) );
}

#endif

