layout(push_constant) uniform PushConstants {
  uint instance;
  uint primitive;
} push_constants;

struct instance_resource_index_type {
  uint world_matrix;
  uint visibility;
  uint aabb;
};

struct primitive_resource_index_type {
  vec4 base_color;
  vec4 emissive;
  float metallic;
  float roughness;
  float normal_scale;
  float occlusion_strength;
  uint base_color_texture;
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

struct resource_pair_type {
  uint inst;
  uint prim;
};

layout(binding = 5,std430) buffer ResourcePairPool {
  resource_pair_type resource_pair[];
};
layout(set = 2, binding = 0) uniform sampler2D texture_pool[];

