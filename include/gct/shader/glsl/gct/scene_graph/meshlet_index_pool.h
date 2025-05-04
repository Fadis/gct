#ifndef GCT_SHADER_SCENE_GRAPH_MESHLET_INDEX_POOL_H
#define GCT_SHADER_SCENE_GRAPH_MESHLET_INDEX_POOL_H

#include <gct/scene_graph/buffer_set_id.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 11, std430) buffer MeshletIndexPool {
  uint meshlet_index_pool[];
};

#endif


