#ifndef GCT_SHADER_SCENE_GRAPH_INSTANCE_RESOURCE_INDEX_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_INSTANCE_RESOURCE_INDEX_TYPE_H

struct instance_resource_index_type {
  uint world_matrix; // このノードの変換行列
  uint inversed_world_matrix;
  uint visibility; // オクルージョンカリングの結果
  uint aabb;
  uint previous_world_matrix;
  uint meshlet_index;
  float coverage;
  uint prim;
  uint lowest_lod;
  uint rigid;
  uint reserved[ 2 ];
};

#endif

