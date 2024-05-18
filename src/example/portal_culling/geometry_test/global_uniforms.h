#ifndef GCT_SHADER_GLOBAL_UNIFORMS_H
#define GCT_SHADER_GLOBAL_UNIFORMS_H

layout(set=1, binding = 16) uniform GlobalUniforms {
  mat4 projection_matrix;
  mat4 camera_matrix;
  mat4 light_vp_matrix0;
  mat4 light_vp_matrix1;
  mat4 light_vp_matrix2;
  mat4 light_vp_matrix3;
  mat4 voxel;
  mat4 inversed_voxel;
  vec4 eye_pos;
  vec4 light_pos;
  float light_energy;
  float light_frustum_width;
  float light_size;
  float split_bias;
  int shadow_mode;
  int frame_counter;
  float ambient;
  float light_z[ 5 ];
} global_uniforms;

layout(std430, set=1, binding = 17) buffer OutputMatrices {
  mat4 camera_projection[6];
  mat4 camera[6];
} matrices;


#endif

