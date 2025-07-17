#ifndef GCT_SHADER_SCENE_GRAPH_DISTANCE_CONSTRAINT_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_DISTANCE_CONSTRAINT_TYPE_H

struct distance_constraint_type {
  uint to_id;
  float stiffness;
  float natural_distance;
  float reserved;
};

#endif

