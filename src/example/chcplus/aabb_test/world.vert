#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
}

