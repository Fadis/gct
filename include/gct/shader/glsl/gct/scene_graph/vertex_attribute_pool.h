#ifndef GCT_SHADER_SCENE_GRAPH_VERTEX_ATTRIBUTE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_VERTEX_ATTRIBUTE_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/vertex_attribute_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 8, std430) buffer VertexAttributePool {
  vertex_attribute_type vertex_attribute[];
};

#endif

