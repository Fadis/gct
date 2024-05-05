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
  aabb_type aabb;
} push_constants;

layout (points) in;
layout (line_strip,max_vertices=36) out;

in gl_PerVertex
{
  vec4 gl_Position;
} gl_in[];

out gl_PerVertex
{
  vec4 gl_Position;
};

vec4 get_aabb_vertex( aabb_type aabb, uint i ) {
  return vec4(
    ( ( i & 4 ) != 0 ) ? aabb.max.x : aabb.min.x,
    ( ( i & 2 ) != 0 ) ? aabb.max.y : aabb.min.y,
    ( ( i & 1 ) != 0 ) ? aabb.max.z : aabb.min.z,
    1.0
  );
}

int vertices[24]=int[](
  0,1,
  1,5,
  5,4,
  4,0,
  2,3,
  3,7,
  7,6,
  6,2,
  0,2,
  4,6,
  1,3,
  5,7
);

void main() {
  gl_PrimitiveID = gl_PrimitiveIDIn;
  const mat4 proj_cam = push_constants.matrix;
  const aabb_type aabb = push_constants.aabb;
  for( int f = 0; f < 12; f++ ) {
    const vec4 v0 = proj_cam * get_aabb_vertex( aabb, vertices[ f * 2 + 0 ] );
    const vec4 v1 = proj_cam * get_aabb_vertex( aabb, vertices[ f * 2 + 1 ] );
    gl_Position = v0;
    EmitVertex();
    gl_Position = v1;
    EmitVertex();
    EndPrimitive();
  }
}

