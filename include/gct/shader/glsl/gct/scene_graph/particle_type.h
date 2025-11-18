#ifndef GCT_SHADER_SCENE_GRAPH_PARTICLE_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_PARTICLE_TYPE_H

struct particle_type {
  vec3 position;
  vec3 previous_position;
  vec3 velocity;
  vec3 local_r;
  vec3 normal;
  float w;
  uint rigid;
  uint phase;
  uint attached;
  float radius;
};

#endif

