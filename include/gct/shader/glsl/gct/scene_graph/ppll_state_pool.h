#ifndef GCT_SHADER_SCENE_GRAPH_PPLL_STATE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_PPLL_STATE_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include<gct/scene_graph/ppll_state.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 22, std430) buffer PPLLStatePool {
  ppll_state ppll_state_pool[];
};

#endif

