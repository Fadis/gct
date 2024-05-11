#version 460
#extension GL_GOOGLE_include_directive : enable
#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 2) in vec4 input_tangent;
layout (location = 3) in vec2 input_texcoord0;

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 2) out vec3 output_tangent;
layout (location = 3) out vec2 output_tex_coord;
layout (location = 4) out vec4 output_shadow0;
layout (location = 5) out vec4 output_shadow1;
layout (location = 6) out vec4 output_shadow2;
layout (location = 7) out vec4 output_shadow3;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout(push_constant) uniform PushConstants {
  uint instance;
  uint primitive;
} push_constants;

struct instance_resource_index_type {
  uint world_matrix;
  uint visibility;
  uint aabb;
}

struct primitive_resource_index_type {
  vec4 color;
  vec4 metallic_roughness;
  vec4 emissive;
  uint color_texture;
  uint metallic_roughness_texture;
  uint normal_texture;
  uint occlusion_texture;
  uint emissive_texture;
};

struct aabb_type {
  vec4 min;
  vec4 max;
};

layout(binding = 0,std430) buffer PrimitiveResourceIndexPool {
  primitive_resource_index_type primitive_resource_index[];
};
layout(binding = 1,std430) buffer InstanceResourceIndexPool {
  instance_resource_index_type instance_resource_index[];
};
layout(binding = 2,std430) buffer VisibilityPool {
  uint visibility_pool[];
};
layout(binding = 3,std430) buffer MatrixPool {
  mat4 matrix_pool[];
};
layout(binding = 4,std430) buffer AABBPool {
  aabb_type aabb_pool[];
};
layout(binding = 5) uniform texture2D texture_pool[];

layout(set=1,binding = 0) uniform GlobalState {
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
  float light_frustum_width;
  float light_size;
  float split_bias;
  int shadow_mode;
  int frame_counter;
  float light_z[ 5 ];
} global_state;

void main() {
  gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
}

