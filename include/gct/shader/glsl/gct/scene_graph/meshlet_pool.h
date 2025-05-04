#ifndef GCT_SHADER_SCENE_GRAPH_MESHLET_POOL_H
#define GCT_SHADER_SCENE_GRAPH_MESHLET_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/meshlet_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 10, std430) buffer MeshletPool {
  meshlet_type meshlet_pool[];
};

#endif

