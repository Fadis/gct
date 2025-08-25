#ifndef GCT_SHADER_SCENE_GRAPH_H
#define GCT_SHADER_SCENE_GRAPH_H

#ifndef GCT_SHADER_SCENE_GRAPH_DISABLE_PUSH_CONSTANT
layout(push_constant) uniform PushConstants {
  uint instance;
  uint primitive;
} push_constants;
#endif

#include <gct/scene_graph/instance_resource_index.h>
#include <gct/scene_graph/primitive_resource_index.h>
#include <gct/scene_graph/visibility_pool.h>
#include <gct/scene_graph/matrix_pool.h>
#include <gct/scene_graph/aabb_pool.h>
#include <gct/scene_graph/resource_pair.h>
#include <gct/scene_graph/texture_pool.h>
#include <gct/scene_graph/image_pool.h>
#include <gct/scene_graph/light_pool.h>
#include <gct/scene_graph/vertex_buffer_pool.h>
#include <gct/scene_graph/accessor_pool.h>
#include <gct/scene_graph/mesh_pool.h>
#include <gct/scene_graph/meshlet_pool.h>
#include <gct/scene_graph/meshlet_index_pool.h>
//#include <gct/scene_graph/draw_request_pool.h>
#include <gct/scene_graph/particle_pool.h>
#include <gct/scene_graph/rigid_pool.h>
#include <gct/scene_graph/distance_constraint_pool.h>
#include <gct/scene_graph/constraint_pool.h>
#include <gct/scene_graph/vertex_to_primitive_pool.h>
#include <gct/scene_graph/lambda_pool.h>
#include <gct/scene_graph/buffer_pool.h>
#include <gct/scene_graph/load_filter_cache.h>
#include <gct/scene_graph/read_primitive.h>
#include <gct/scene_graph/k+buffer.h>

#endif

