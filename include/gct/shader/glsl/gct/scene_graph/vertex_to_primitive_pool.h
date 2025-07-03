#ifndef GCT_SHADER_SCENE_GRAPH_VERTEX_TO_PRIMITIVE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_VERTEX_TO_PRIMITIVE_POOL_H

#include <gct/scene_graph/buffer_set_id.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 15, std430) buffer VertexToPrimitivePool {
  uint vertex_to_primitive_pool[];
};

#endif

