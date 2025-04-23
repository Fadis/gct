#ifndef GCT_SHADER_SCENE_GRAPH_DRAW_REQUEST_POOL_H
#define GCT_SHADER_SCENE_GRAPH_DRAW_REQUEST_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/scene_graph/draw_request_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 11, std430) buffer DrawRequestPool {
  draw_request_type draw_request_pool[];
};

#endif


