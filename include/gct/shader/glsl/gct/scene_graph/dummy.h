#ifndef GCT_SHADER_SCENE_GRAPH_DUMMY_H
#define GCT_SHADER_SCENE_GRAPH_DUMMY_H

#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

#include <gct/scene_graph.h>
#include <gct/global_uniforms.h>
#include <gct/scene_graph/spatial_hash.h>

layout (std430, binding = 7) buffer Tone {
  uint value;
  float scale;
} tone;

layout (std430, binding = 13) buffer AFState {
  uint history;
  uint max_history;
  float depth;
  float znear;
  float zfar;
  float focal_distance;
  float lens_size;
  float sensor_size;
  float apeture_rotation;
  float visible_range;
} af_state;

layout(set=1, binding = 18) uniform SpatialHashConfig {
  hash_table_type config;
} spatial_hash_config;

#endif

