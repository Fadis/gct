#ifndef GCT_SHADER_GET_TANGENT_H
#define GCT_SHADER_GET_TANGENT_H

vec3 get_tangent() {
  return normalize( input_tangent.xyz );
}

#endif
