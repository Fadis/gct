#ifndef GCT_SHADER_SCENE_GRAPH_MESH_POOL_H
#define GCT_SHADER_SCENE_GRAPH_MESH_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/mesh_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 9, std430) buffer MeshPool {
  mesh_type mesh_pool[];
};

#endif


