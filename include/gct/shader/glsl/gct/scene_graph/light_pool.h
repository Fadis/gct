#ifndef GCT_SHADER_SCENE_GRAPH_LIGHT_POOL_H
#define GCT_SHADER_SCENE_GRAPH_LIGHT_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/punctual_light_parameter.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 6, std430) buffer LightPool {
  punctual_light_parameter light_pool[];
};

#endif

