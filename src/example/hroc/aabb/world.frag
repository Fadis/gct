#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include "global_uniforms.h"
#include "scene_graph.h"

const uint null_node = 0xFFFFFFFF;

struct node_type {
  aabb_type aabb;
  uint parent;
  uint left;
  uint right;
  uint leaf;
};

layout(std430, binding = 8) buffer BVH {
  node_type bvh[];
};

layout(std430, binding = 9) buffer Visibility {
  uint visibility[];
};

layout(early_fragment_tests) in;

layout ( location = 0 ) flat in uint input_instance_id;
layout ( location = 1 ) in vec4 input_position;

bool intersect_aabb3( aabb_type aabb, vec4 p0, vec4 p1 ) {
  vec3 tmin = ( aabb.min.xyz - p0.xyz ) / ( p1.xyz - p0.xyz );
  vec3 tmax = ( aabb.max.xyz - p0.xyz ) / ( p1.xyz - p0.xyz );
  vec3 t1 = min( tmin, tmax );
  vec3 t2 = max( tmin, tmax );
  float tnear = max( max( t1.x, t1.y ), t1.z );
  float tfar = min( min( t2.x, t2.y ), t2.z );
  return tnear <= tfar;
}
#define CHECK_CHILD( cur_name, next_name ) \
void cur_name ( \
  uint cur_id, \
  vec4 p0, \
  vec4 p1 \
) { \
  const node_type cur = bvh[ cur_id ]; \
  if( !intersect_aabb3( cur.aabb, p0, p1 ) ) { \
    return; \
  } \
  if( cur.leaf != null_node ) { \
    visibility_pool[ instance_resource_index[ cur.leaf ].visibility ] = 1; \
    return; \
  } \
  if( cur.left != null_node ) { \
    next_name ( cur.left, p0, p1 ); \
  } \
  if( cur.right != null_node ) { \
    next_name ( cur.right, p0, p1 ); \
  } \
}

void check_child11(
  uint cur_id,
  vec4 p0,
  vec4 p1
) {}

CHECK_CHILD( check_child10, check_child11 )
CHECK_CHILD( check_child9, check_child10 )
CHECK_CHILD( check_child8, check_child9 )
CHECK_CHILD( check_child7, check_child8 )
CHECK_CHILD( check_child6, check_child7 )
CHECK_CHILD( check_child5, check_child6 )
CHECK_CHILD( check_child4, check_child5 )
CHECK_CHILD( check_child3, check_child4 )
CHECK_CHILD( check_child2, check_child3 )
CHECK_CHILD( check_child1, check_child2 )
CHECK_CHILD( check_child0, check_child1 )

void main()  {
  vec4 p0 = global_uniforms.eye_pos;
  vec4 p1 = input_position / input_position.w;
  check_child0( input_instance_id, p0, p1 );
}

