#ifndef GCT_SHADER_SCENE_GRAPH_TEXTURE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_TEXTURE_POOL_H

#include <gct/scene_graph/image_metadata_type.h>

#ifndef GCT_SHADER_SCENE_GRAPH_TEXTURE_SET_ID
#define GCT_SHADER_SCENE_GRAPH_TEXTURE_SET_ID 2
#endif

#define texture_pool_2d texture_pool

layout(set = GCT_SHADER_SCENE_GRAPH_TEXTURE_SET_ID, binding = 0) uniform sampler1D texture_pool_1d[];
layout(set = GCT_SHADER_SCENE_GRAPH_TEXTURE_SET_ID, binding = 0) uniform sampler2D texture_pool[];
layout(set = GCT_SHADER_SCENE_GRAPH_TEXTURE_SET_ID, binding = 0) uniform sampler3D texture_pool_3d[];
layout(set = GCT_SHADER_SCENE_GRAPH_TEXTURE_SET_ID, binding = 0) uniform samplerCube texture_pool_cube[];

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 24, std430) buffer TextureMetadataPool {
  image_metadata_type texture_metadata_pool[];
};


#endif

