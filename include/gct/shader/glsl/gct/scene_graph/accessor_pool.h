#ifndef GCT_SHADER_SCENE_GRAPH_ACCESSOR_POOL_H
#define GCT_SHADER_SCENE_GRAPH_ACCESSOR_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/accessor_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 7, std430) buffer AccessorPool {
  accessor_type accessor[];
};

#endif

