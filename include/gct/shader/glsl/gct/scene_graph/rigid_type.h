#ifndef GCT_SHADER_SCENE_GRAPH_RIGID_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_RIGID_TYPE_H

struct rigid_type {
  vec4 linear_velocity;
  vec4 angular_velocity;
  vec4 previous_linear_velocity;
  vec4 previous_angular_velocity;
  vec4 local_center_of_mass;
  uint trs;
  uint trs_previous;
  uint trs_previous_coarse;
  uint momentum_inertia_tensor;
  uint inversed_momentum_inertia_tensor;
  float mass;
  uint collision_constraint_offset;
  uint constraint_count;
};

#endif


