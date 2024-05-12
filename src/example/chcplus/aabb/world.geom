#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable

struct aabb_type {
  vec4 min;
  vec4 max;
};

layout(push_constant) uniform PushConstants {
  mat4 matrix;
  uint head;
} push_constants;

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

layout(binding = 7,std430) buffer AABBPool {
  aabb_type aabb[];
};

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

void main() {
  gl_PrimitiveID = gl_PrimitiveIDIn;
  const mat4 proj_cam = push_constants.matrix;
  const aabb_type aabb = aabb[ push_constants.head + input_instance_id[ 0 ] ];
  for( int f = 0; f < 12; f++ ) {
    const vec4 v0 = proj_cam * get_aabb_vertex( aabb, vertices[ f * 3 + 0 ] );
    const vec4 v1 = proj_cam * get_aabb_vertex( aabb, vertices[ f * 3 + 1 ] );
    const vec4 v2 = proj_cam * get_aabb_vertex( aabb, vertices[ f * 3 + 2 ] );
    gl_Position = v0;
    EmitVertex();
    gl_Position = v1;
    EmitVertex();
    gl_Position = v2;
    EmitVertex();
    EndPrimitive();
  }
}

