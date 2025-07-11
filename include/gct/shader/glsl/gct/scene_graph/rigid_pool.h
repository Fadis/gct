#ifndef GCT_SHADER_SCENE_GRAPH_RIGID_POOL_H
#define GCT_SHADER_SCENE_GRAPH_RIGID_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include<gct/scene_graph/rigid_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 16, std430) buffer RigidPool {
  rigid_type rigid_pool[];
};

#endif

