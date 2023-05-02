layout(push_constant) uniform PushConstants {
  mat4 world_matrix;
  int fid;
} push_constants;

layout(binding = 0) uniform Uniforms {
  vec4 base_color;
  vec4 emissive;
  float roughness;
  float metalness;
  float normal_scale;
  float occlusion_strength;
} uniforms;

layout(set=1,binding = 0) uniform DynamicUniforms {
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
  float light_z[ 5 ];
} dynamic_uniforms;

