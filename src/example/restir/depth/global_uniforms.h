#ifndef GCT_SHADER_GLOBAL_UNIFORMS_H
#define GCT_SHADER_GLOBAL_UNIFORMS_H

layout(set=1, binding = 16) uniform GlobalUniforms {
  vec4 eye_pos;
  ivec4 voxel_proj;
  int projection_matrix;
  int camera_matrix;
  int previous_projection_matrix;
  int previous_camera_matrix;
  int screen_to_world_matrix;
  int voxel;
  int inversed_voxel;
  int frame_counter;
  int light_count;
  float ambient;
} global_uniforms;

layout(std430, set=1, binding = 17) buffer OutputMatrices {
  mat4 camera_projection[6];
  mat4 camera[6];
} matrices;

#endif

