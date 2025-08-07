#ifndef GCT_SHADER_LAMBDA_H
#define GCT_SHADER_LAMBDA_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>

uint distance_lambda_begin(
  uint particle_id,
  uint id_offset
) {
  return id_offset + particle_id * 128;
}

uint distance_lambda_next(
  uint gcid
) {
  return gcid + 1;
}

float distance_lambda_get(
  uint gcid
) {
  return lambda_pool[ gcid ];
}

void distance_lambda_set(
  uint gcid,
  float v
) {
  lambda_pool[ gcid ] = v;
}

uint rigid_lambda_begin(
  uint id_offset
) {
  return id_offset;
}

uint rigid_lambda_next(
  uint gcid
) {
  return gcid + 1;
}

float rigid_lambda_get(
  uint gcid
) {
  return lambda_pool[ gcid ];
}

void rigid_lambda_set(
  uint gcid,
  float v
) {
  lambda_pool[ gcid ] = v;
}

#endif

