#ifndef GCT_SHADER_SCENE_GRAPH_PARTICLE_POOL_H
#define GCT_SHADER_SCENE_GRAPH_PARTICLE_POOL_H

#include <gct/scene_graph/buffer_set_id.h>
#include<gct/scene_graph/particle_type.h>

layout(set = GCT_SHADER_SCENE_GRAPH_BUFFER_SET_ID, binding = 12, std430) buffer ParticlePool {
  particle_type particle_pool[];
};

#endif

