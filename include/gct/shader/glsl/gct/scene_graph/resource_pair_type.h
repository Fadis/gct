#ifndef GCT_SHADER_SCENE_GRAPH_RESOURCE_PAIR_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_RESOURCE_PAIR_TYPE_H

struct resource_pair_type {
  uint inst; // ノードにぶら下がるプリミティブの情報を指すID
  uint prim; // プリミティブの情報を指すID
  uint offset;
  uint reserved;
};

#endif

