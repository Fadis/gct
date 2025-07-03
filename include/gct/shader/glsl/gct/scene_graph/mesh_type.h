#ifndef GCT_SHADER_SCENE_GRAPH_MESH_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_MESH_TYPE_H

struct mesh_type {
  uint accessor; // 最初のアクセサのオフセット
  uint vertex_count; // 頂点数
  uint topology; // トポロジ
  uint meshlet; // メッシュレット単位の情報
  uint unique_vertex_count;
  uint particle_offset;
  uint distance_constraint_offset;
  uint constraint_offset;
  uint vertex_to_primitive_offset;
  float stiffness;
  uint reserved[ 2 ];
};

#endif

