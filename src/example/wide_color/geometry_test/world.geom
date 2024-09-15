#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable

#include "scene_graph.h"
#include "global_uniforms.h"

layout (triangles) in;
layout (triangle_strip,max_vertices=18) out;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec4 input_position[];
layout (location = 1) in vec3 input_normal[];
layout (location = 3) in vec2 input_texcoord[];

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 3) out vec2 output_texcoord;


vec4 unit_triangle[3]=vec4[](
  vec4( 0.0, 0.0, 0.0, 1.0 ),
  vec4( 1.0, 1.0, 0.0, 1.0 ),
  vec4( 1.0, 0.0, 0.0, 1.0 )
);

void main() {
  mat4 world_matrix = matrix_pool[ instance_resource_index[ push_constants.instance ].world_matrix ];
  gl_PrimitiveID = gl_PrimitiveIDIn;
  for( int i = 0; i < gl_in.length(); i++ ) {
    gl_Position = global_uniforms.projection_matrix * global_uniforms.camera_matrix * world_matrix * unit_triangle[ i ];
    output_position = global_uniforms.camera_matrix * world_matrix * unit_triangle[ i ];
    output_normal = input_normal[ i ];
    output_texcoord = input_texcoord[ i ];
    EmitVertex();
  }
  EndPrimitive();
}

