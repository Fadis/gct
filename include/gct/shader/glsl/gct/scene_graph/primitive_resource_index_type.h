#ifndef GCT_SHADER_SCENE_GRAPH_PRIMITIVE_RESOURCE_INDEX_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_PRIMITIVE_RESOURCE_INDEX_TYPE_H

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
  uint mesh;
  uint reserved[ 2 ];
};

#endif

