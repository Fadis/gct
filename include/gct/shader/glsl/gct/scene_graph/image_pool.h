#ifndef GCT_SHADER_SCENE_GRAPH_IMAGE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_IMAGE_POOL_H

#ifndef GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID
#define GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID 3
#endif

#ifdef GCT_MAKE_IMAGE_COHERENT
#define GCT_IMAGE_COHERENT_QUALIFIER coherent
#else
#define GCT_IMAGE_COHERENT_QUALIFIER
#endif

#ifdef GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform image1D image_pool_1d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_pool_2d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform image3D image_pool_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform image1DArray image_pool_1d_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_pool_2d_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage1D image_pool_1du[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage2D image_pool_2du[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage3D image_pool_3du[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage1DArray image_pool_1du_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage2DArray image_pool_2du_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32i) GCT_IMAGE_COHERENT_QUALIFIER uniform iimage1D image_pool_1da[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32i) GCT_IMAGE_COHERENT_QUALIFIER uniform iimage2D image_pool_2da[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32i) GCT_IMAGE_COHERENT_QUALIFIER uniform iimage3D image_pool_3da[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32i) GCT_IMAGE_COHERENT_QUALIFIER uniform iimage1DArray image_pool_1da_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32i) GCT_IMAGE_COHERENT_QUALIFIER uniform iimage2DArray image_pool_2da_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32ui) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage1D image_pool_1dua[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32ui) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage2D image_pool_2dua[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32ui) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage3D image_pool_3dua[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32ui) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage1DArray image_pool_1dua_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32ui) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage2DArray image_pool_2dua_array[];

#endif

layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba8) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_pool8[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_pool16[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_pool16f[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba32f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_pool32f[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r8) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_poolr8[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r16f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_poolr16f[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_poolr32f[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba8) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_pool8_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_pool16_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_pool16f_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba32f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_pool32f_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r8) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_poolr8_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r16f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_poolr16f_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2DArray image_poolr32f_array[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba8) GCT_IMAGE_COHERENT_QUALIFIER uniform image3D image_pool8_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16) GCT_IMAGE_COHERENT_QUALIFIER uniform image3D image_pool16_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba16f) GCT_IMAGE_COHERENT_QUALIFIER uniform image3D image_pool16f_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, rgba32f) GCT_IMAGE_COHERENT_QUALIFIER uniform image3D image_pool32f_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r16f) GCT_IMAGE_COHERENT_QUALIFIER uniform image3D image_poolr16f_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r32f) GCT_IMAGE_COHERENT_QUALIFIER uniform image2D image_poolr32f_3d[];
layout (set = GCT_SHADER_SCENE_GRAPH_IMAGE_SET_ID, binding = 0, r8ui) GCT_IMAGE_COHERENT_QUALIFIER uniform uimage2D image_poolr8u[];

#endif

