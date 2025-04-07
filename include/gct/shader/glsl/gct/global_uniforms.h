#ifndef GCT_SHADER_GLOBAL_UNIFORMS_H
#define GCT_SHADER_GLOBAL_UNIFORMS_H

#ifndef GCT_SHADER_SCENE_GRAPH_GLOBAL_UNIFORMS_SET_ID
#define GCT_SHADER_SCENE_GRAPH_GLOBAL_UNIFORMS_SET_ID 1
#endif

#ifndef GCT_SHADER_SCENE_GRAPH_GLOBAL_UNIFORMS_BINDING_ID
#define GCT_SHADER_SCENE_GRAPH_GLOBAL_UNIFORMS_BINDING_ID 16
#endif

layout(set=GCT_SHADER_SCENE_GRAPH_GLOBAL_UNIFORMS_SET_ID, binding = GCT_SHADER_SCENE_GRAPH_GLOBAL_UNIFORMS_BINDING_ID) uniform GlobalUniforms {
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
  int gbuffer;
  int depth;
} global_uniforms;

#endif

