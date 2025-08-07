#ifndef GCT_SHADER_SCENE_GRAPH_RIGID_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_RIGID_TYPE_H

struct rigid_type {
  vec3 center_of_mass;
  uint inversed_momentum_inertia_tensor;
  vec3 previous_center_of_mass;
  vec3 linear_velocity;
  vec4 angular_orientation;
  vec4 previous_angular_orientation;
  vec4 angular_velocity;
};

#endif


