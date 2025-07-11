#ifndef GCT_SHADER_SCENE_GRAPH_SPATIAL_HASH_POOL_H
#define GCT_SHADER_SCENE_GRAPH_SPATIAL_HASH_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/spatial_hash_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 17, std430) buffer SpatialHashPool {
  spatial_hash_type spatial_hash_pool[];
};

#endif

