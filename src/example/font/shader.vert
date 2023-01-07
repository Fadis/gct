#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout(binding = 0) uniform Uniforms {
  mat4 world_matrix;
  mat2 char_matrix[ 1024 ];
} uniforms;

layout (location = 0) in vec3 input_position;
out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  gl_Position = uniforms.world_matrix * local_pos;
}

