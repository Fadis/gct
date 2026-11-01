#ifndef GCT_SHADER_SOBEL_FILTER_H
#define GCT_SHADER_SOBEL_FILTER_H

#ifdef __cplusplus
#include <gct/shader/glsl/gct/neighbor.h>
#else
#include <gct/neighbor.h>
#endif

#ifdef __cplusplus
const float sobel_filter_xgrad[moore_neighbor_count] = {
#else
const float sobel_filter_xgrad[moore_neighbor_count] = float[](
#endif
  -1.0f,  0.0f,  1.0f,
  -2.0f,         2.0f,
  -1.0f,  0.0f,  1.0f
#ifdef __cplusplus
};
#else
);
#endif

#ifdef __cplusplus
const float sobel_filter_ygrad[moore_neighbor_count] = {
#else
const float sobel_filter_ygrad[moore_neighbor_count] = float[](
#endif
   1.0f,  2.0f,  1.0f,
   0.0f,         0.0f,
  -1.0f, -2.0f, -1.0f
#ifdef __cplusplus
};
#else
);
#endif

#endif

