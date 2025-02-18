#ifndef GCT_SHADER_SCENE_GRAPH_RESOURCE_PAIR_H
#define GCT_SHADER_SCENE_GRAPH_RESOURCE_PAIR_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/resource_pair_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 5, std430) buffer ResourcePairPool {
  resource_pair_type resource_pair[];
};

#endif

