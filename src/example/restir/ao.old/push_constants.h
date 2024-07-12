#ifndef GCT_SHADER_PUSH_CONSTANTS_H
#define GCT_SHADER_PUSH_CONSTANTS_H

layout(binding = 4) uniform DynamicUniforms {
  vec4 eye_pos;
  vec4 light_pos;
  ivec4 voxel_proj;
  int projection_matrix;
  int camera_matrix;
  int previous_projection_matrix;
  int previous_camera_matrix;
  int screen_to_world_matrix;
  int voxel;
  int inversed_voxel;
  float light_energy;
  float light_frustum_width;
  float light_size;
  int frame_counter;
  float ambient;
} dynamic_uniforms;

#endif

