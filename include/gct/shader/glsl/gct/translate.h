#ifndef GCT_SHADER_TRANSLATE_H
#define GCT_SHADER_TRANSLATE_H

mat4 translate(
  vec3 t
) {
  return mat4(
    1, 0, 0, 0,
    0, 1, 0, 0,
    0, 0, 1, 0,
    t.x, t.y, t.z, 1
  );
}

mat4 lookat(
  vec3 eye,
  vec3 center,
  vec3 up
) {
  const vec3 f = normalize( center - eye );
  const vec3 up_ = normalize( up );
  const vec3 s = cross( f, up_ );
  const vec3 u = cross( normalize( s ), f );
  return
    mat4(
      s.x, u.x, -f.x, 0,
      s.y, u.y, -f.y, 0,
      s.z, u.z, -f.z, 0,
        0,   0,    0, 1
    ) *
    translate( -eye );
}

mat4 perspective(
  float fovy,
  float aspect,
  float near,
  float far
) {
  const float f = 1.0 / tan( fovy / 2.0 );
  const float a = ( far + near )/( near -far );
  const float b = ( 2 * far * near )/( near - far );
  return mat4(
    f/aspect,  0,  0,  0,
    0,         f,  0,  0,
    0,         0,  a, -1,
    0,         0,  b,  0
  );
}

mat4 perspective_simple(
  float near,
  float far
) {
  const float a = ( far + near )/( near -far );
  const float b = ( 2 * far * near )/( near - far );
  return mat4(
    1,  0,  0,  0,
    0,  1,  0,  0,
    0,  0,  a, -1,
    0,  0,  b,  0  
  );
}

#endif

