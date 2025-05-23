#ifndef GCT_SHADER_SCENE_GRAPH_MESHLET_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_MESHLET_TYPE_H

struct meshlet_type {
  vec4 center; // 頂点座標の平均と全ての頂点を内包する球の半径
  vec4 normal; // 法錐の軸と-sin(alpha)
};

#endif

