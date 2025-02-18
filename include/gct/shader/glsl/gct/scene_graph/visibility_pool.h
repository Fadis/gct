#ifndef GCT_SHADER_SCENE_GRAPH_VISIBILITY_POOL_H
#define GCT_SHADER_SCENE_GRAPH_VISIBILITY_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 2, std430) buffer VisibilityPool {
  uint visibility_pool[];
};

#endif

