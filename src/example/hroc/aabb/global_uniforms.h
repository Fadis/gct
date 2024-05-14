#ifndef GCT_SHADER_GLOBAL_UNIFORMS_H
#define GCT_SHADER_GLOBAL_UNIFORMS_H

layout(binding = 16,std430) buffer GlobalUniforms {
  mat4 projection_matrix;
  mat4 camera_matrix;
  vec4 eye_pos;
} global_uniforms;

#endif

