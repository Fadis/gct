#ifndef GCT_SHADER_SCENE_GRAPH_MESHLET_TYPE_H
#define GCT_SHADER_SCENE_GRAPH_MESHLET_TYPE_H

struct meshlet_type {
  vec4 center; // position.xyz radius
  vec4 normal; // normal.xyz max_angle
};

#endif

