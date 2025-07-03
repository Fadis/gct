#ifndef GCT_SHADER_SCENE_GRAPH_DISTANCE_CONSTRAINT_POOL_H
#define GCT_SHADER_SCENE_GRAPH_DISTANCE_CONSTRAINT_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include<gct/scene_graph/distance_constraint_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 13, std430) buffer DistanceConstraintPool {
  distance_constraint_type distance_constraint_pool[];
};

#endif

