#ifndef GCT_SHADER_SCENE_GRAPH_MESH_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_MESH_TYPE_H

struct mesh_type {
  uint accessor;
  uint vertex_count;
  uint topology;
  float occupancy;
  uint meshlet;
  uint reserved[ 3 ];
};

#endif

