#ifndef GCT_SHADER_SCENE_GRAPH_IMAGE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_IMAGE_POOL_H

#ifndef GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID
#define GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID 3
#endif

layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba8) uniform image2D image_pool8[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16) uniform image2D image_pool16[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16f) uniform image2D image_pool16f[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba32f) uniform image2D image_pool32f[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba8) uniform image2DArray image_pool8_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16) uniform image2DArray image_pool16_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16f) uniform image2DArray image_pool16f_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba32f) uniform image2DArray image_pool32f_array[];

#endif

