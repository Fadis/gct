#ifndef GCT_SHADER_SCENE_GRAPH_PRIMITIVE_RESOURCE_INDEX_H
#define GCT_SHADER_SCENE_GRAPH_PRIMITIVE_RESOURCE_INDEX_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/primitive_resource_index_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 0, std430) buffer PrimitiveResourceIndexPool {
  primitive_resource_index_type primitive_resource_index[];
};

#endif

