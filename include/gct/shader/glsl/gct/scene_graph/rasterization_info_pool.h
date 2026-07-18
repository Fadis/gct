#ifndef GCT_SHADER_SCENE_GRAPH_RASTERIZATION_INFO_POOL_H
#define GCT_SHADER_SCENE_GRAPH_RASTERIZATION_INFO_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include<gct/rasterize.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 25, std430) buffer RasterizationInfoPool {
  rasterization_info rasterization_info_pool[];
};

#endif


