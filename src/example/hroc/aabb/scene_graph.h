layout(push_constant) uniform PushConstants {
  uint src_count;
  uint src_offset;
  uint dest_count;
  uint dest_offset;
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

layout(binding = 1,std430) buffer InstanceResourceIndexPool {
  instance_resource_index_type instance_resource_index[];
};
layout(binding = 2,std430) buffer VisibilityPool {
  uint visibility_pool[];
};
layout(binding = 4,std430) buffer AABBPool {
  aabb_type aabb_pool[];
};

#include <gct/scene_graph/resource_pair_type.h>

layout(binding = 5,std430) buffer ResourcePairPool {
  resource_pair_type resource_pair[];
};

