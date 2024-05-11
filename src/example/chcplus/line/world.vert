#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_position;

layout(push_constant) uniform PushConstants {
  mat4 matrix;
} push_constants;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  gl_Position = push_constants.matrix * input_position;
}

