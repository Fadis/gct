#ifndef GCT_SHADER_PUSH_CONSTANTS_H
#define GCT_SHADER_PUSH_CONSTANTS_H

layout(binding=3) uniform GlobalUniforms {
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
  int light;
} global_uniforms;

#endif

