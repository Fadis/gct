#ifndef GCT_SHADER_SCENE_GRAPH_AABB_POOL_H
#define GCT_SHADER_SCENE_GRAPH_AABB_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include<gct/aabb_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 4, std430) buffer AABBPool {
  aabb_type aabb_pool[];
};

#endif

