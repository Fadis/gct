#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable

layout (triangles) in;
layout (triangle_strip,max_vertices=18) out;

layout(std430, set=1, binding = 17) buffer OutputMatrices {
  mat4 camera_projection[6];
  mat4 camera[6];
} matrices;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec4 input_position[];

layout (location = 0) out vec4 output_position;

void main() {
  gl_PrimitiveID = gl_PrimitiveIDIn;
  for( int l = 0; l < 6; l++ ) {
    gl_Layer = l;
    for( int i = 0; i < gl_in.length(); i++ ) {
      gl_Position = matrices.camera_projection[ l ] * gl_in[ i ].gl_Position;
      output_position = matrices.camera[ l ] * input_position[ i ];
      EmitVertex();
    }
    EndPrimitive();
  }
}

