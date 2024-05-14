#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable

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

layout(binding = 6,set=1,std430) buffer Condition {
  uint condition[];
};

layout (points) in;
layout (triangle_strip,max_vertices=36) out;

in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[];

out gl_PerVertex
{
  vec4 gl_Position;
};

layout ( location = 0 ) flat in uint input_instance_id[];
layout ( location = 0 ) flat out uint output_instance_id;
layout ( location = 1 ) out vec4 output_position;

vec4 get_aabb_vertex( aabb_type aabb, uint i ) {
  return vec4(
    ( ( i & 4 ) != 0 ) ? aabb.max.x : aabb.min.x,
    ( ( i & 2 ) != 0 ) ? aabb.max.y : aabb.min.y,
    ( ( i & 1 ) != 0 ) ? aabb.max.z : aabb.min.z,
    1.0
  );
}

int vertices[36]=int[](
  0,1,5,
  0,5,4,
  3,2,6,
  3,6,7,

  0,2,3,
  0,3,1,
  5,7,6,
  5,6,4,
  
  4,6,2,
  4,2,0,
  1,3,7,
  1,7,5
);

aabb_type aabb_and3( aabb_type l, aabb_type r ) {
  aabb_type result; 
  result.min.x = max( l.min.x, r.min.x );
  result.min.y = max( l.min.y, r.min.y );
  result.min.z = max( l.min.z, r.min.z );
  result.max.x = min( l.max.x, r.max.x );
  result.max.y = min( l.max.y, r.max.y );
  result.max.z = min( l.max.z, r.max.z );
  result.min.x = min( result.min.x, result.max.x );
  result.min.y = min( result.min.y, result.max.y );
  result.min.z = min( result.min.z, result.max.z );
  return result;
}

float aabb_area3( aabb_type r ) {
  return
    ( r.max.x - r.min.x ) *
    ( r.max.y - r.min.y ) *
    ( r.max.z - r.min.z );
}


void main() {
  const uint cur_v = visibility[ input_instance_id[ 0 ] ];
  const node_type cur = bvh[ input_instance_id[ 0 ] ];
  const mat4 proj_cam = global_uniforms.projection_matrix * global_uniforms.camera_matrix;
  if( cur_v == 0 ) {
    if( cur.parent == null_node ) {
      mat4 inv = inverse( proj_cam );
      const vec4 v0 = vec4( -1.0, -1.0, 0.0, 1.0 );
      const vec4 v1 = vec4( -1.0,  1.0, 0.0, 1.0 );
      const vec4 v2 = vec4(  1.0, -1.0, 0.0, 1.0 );
      const vec4 v3 = vec4(  1.0,  1.0, 0.0, 1.0 );
      gl_Position = v0;
      output_instance_id = input_instance_id[ 0 ];
      output_position = inv * v0;
      EmitVertex();
      gl_Position = v1;
      output_instance_id = input_instance_id[ 0 ];
      output_position = inv * v1;
      EmitVertex();
      gl_Position = v2;
      output_instance_id = input_instance_id[ 0 ];
      output_position = inv * v2;
      EmitVertex();
      gl_Position = v2;
      output_instance_id = input_instance_id[ 0 ];
      output_position = inv * v2;
      EmitVertex();
      gl_Position = v1;
      output_instance_id = input_instance_id[ 0 ];
      output_position = inv * v1;
      EmitVertex();
      gl_Position = v3;
      output_instance_id = input_instance_id[ 0 ];
      output_position = inv * v3;
      EmitVertex();
      EndPrimitive();
    }
    return;
  }
  if( cur.leaf != null_node ) {
    return;
  }
  gl_PrimitiveID = gl_PrimitiveIDIn;
  const float znear = abs( global_uniforms.projection_matrix[ 3 ][ 2 ] / ( -1.0 + global_uniforms.projection_matrix[ 2 ][ 2 ] ) );
  aabb_type near_box;
  near_box.min = global_uniforms.eye_pos - vec4( 2.0 * znear, 2.0 * znear, 2.0 * znear, 0.0 );
  near_box.max = global_uniforms.eye_pos + vec4( 2.0 * znear, 2.0 * znear, 2.0 * znear, 0.0 );
  const uint left_index = cur.left;
  const uint right_index = cur.right;

  uint invisible_node = 0u;

  if( left_index != null_node && right_index != null_node ) {
    const uint left_v = visibility[ left_index ];
    const uint right_v = visibility[ right_index ];
    if( left_v == 0 && right_v != 0 ) {
      invisible_node = left_index;
    }
    else if( left_v != 0 && right_v == 0 ) {
      invisible_node = right_index;
    }
    else {
      return;
    }
  }
  else if( left_index != null_node ) {
    const uint left_v = visibility[ left_index ];
    if( left_v == 0 ) {
      invisible_node = left_index;
    }
  }

  const aabb_type aabb = bvh[ invisible_node ].aabb;
  
  if( aabb_area3( aabb_and3( aabb, near_box ) ) != 0.0 ) {
    mat4 inv = inverse( proj_cam );
    const vec4 v0 = vec4( -1.0, -1.0, 0.0, 1.0 );
    const vec4 v1 = vec4( -1.0,  1.0, 0.0, 1.0 );
    const vec4 v2 = vec4(  1.0, -1.0, 0.0, 1.0 );
    const vec4 v3 = vec4(  1.0,  1.0, 0.0, 1.0 );
    gl_Position = v0;
    output_instance_id = invisible_node;
    output_position = inv * v0;
    EmitVertex();
    gl_Position = v1;
    output_instance_id = invisible_node;
    output_position = inv * v1;
    EmitVertex();
    gl_Position = v2;
    output_instance_id = invisible_node;
    output_position = inv * v2;
    EmitVertex();
    gl_Position = v2;
    output_instance_id = invisible_node;
    output_position = inv * v2;
    EmitVertex();
    gl_Position = v1;
    output_instance_id = invisible_node;
    output_position = inv * v1;
    EmitVertex();
    gl_Position = v3;
    output_instance_id = invisible_node;
    output_position = inv * v3;
    EmitVertex();
    EndPrimitive();
  }
  else {
    for( int f = 0; f < 12; f++ ) {
      vec4 v0 = get_aabb_vertex( aabb, vertices[ f * 3 + 0 ] );
      vec4 v1 = get_aabb_vertex( aabb, vertices[ f * 3 + 1 ] );
      vec4 v2 = get_aabb_vertex( aabb, vertices[ f * 3 + 2 ] );
      gl_Position = proj_cam * v0;
      output_instance_id = invisible_node;
      output_position = v0;
      EmitVertex();
      gl_Position = proj_cam * v1;
      output_instance_id = invisible_node;
      output_position = v1;
      EmitVertex();
      gl_Position = proj_cam * v2;
      output_instance_id = invisible_node;
      output_position = v2;
      EmitVertex();
      EndPrimitive();
    }
  }
}

