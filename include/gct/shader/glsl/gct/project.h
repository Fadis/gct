#ifndef GCT_SHADER_PROJECT_H
#define GCT_SHADER_PROJECT_H

vec2 project( vec2 v, vec2 n  ) {
  return v - dot( v, n ) * n;
}

vec3 project( vec3 v, vec3 n  ) {
  return v - dot( v, n ) * n;
}

#endif

