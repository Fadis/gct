#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform Uniforms {
  mat4 world_matrix;
  mat4 rect_matrix;
} uniforms;

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec2 input_texcoord;
layout (location = 0) out vec2 output_texcoord;
out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  gl_Position = uniforms.world_matrix * uniforms.rect_matrix * local_pos;
  output_texcoord = input_texcoord;
  //output_texcoord = ( uniforms.rect_matrix * vec4( input_texcoord, 0.f, 1.f ) ).xy;
}

