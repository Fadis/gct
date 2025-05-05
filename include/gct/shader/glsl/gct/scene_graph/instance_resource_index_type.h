#ifndef GCT_SHADER_SCENE_GRAPH_INSTANCE_RESOURCE_INDEX_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_INSTANCE_RESOURCE_INDEX_TYPE_H

struct instance_resource_index_type {
  uint world_matrix;
  uint visibility;
  uint aabb;
  uint previous_world_matrix;
  uint meshlet_index;
  uint lod;
  uint reserved[2];
};

#endif

