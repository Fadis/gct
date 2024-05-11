#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;

out gl_PerVertex
{
    vec4 gl_Position;
};

layout ( location = 0 ) flat out uint output_instance_id;

void main() {
  gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
  output_instance_id = gl_InstanceIndex;
}

