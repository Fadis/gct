#ifndef GCT_SHADER_SCENE_GRAPH_RIGID_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_RIGID_TYPE_H

struct rigid_type {
  vec4 linear_velocity;
  vec4 angular_velocity;
  vec4 local_center_of_mass;
  uint trs;
  uint trs_previous;
  uint inversed_momentum_inertia_tensor;
  uint reserved[ 3 ];
};

#endif


