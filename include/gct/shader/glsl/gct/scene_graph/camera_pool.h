#ifndef GCT_SHADER_SCENE_GRAPH_CAMERA_POOL_H
#define GCT_SHADER_SCENE_GRAPH_CAMERA_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include <gct/camera_state.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 20, std430) buffer CameraPool {
  camera_state camera_pool[];
};

#endif


