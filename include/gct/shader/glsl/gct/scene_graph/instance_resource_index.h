#ifndef GCT_SHADER_SCENE_GRAPH_INSTANCE_RESOURCE_INDEX_H
#define GCT_SHADER_SCENE_GRAPH_INSTANCE_RESOURCE_INDEX_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/instance_resource_index_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 1, std430) buffer InstanceResourceIndexPool {
  instance_resource_index_type instance_resource_index[];
};

#endif

