#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_shader_image_load_formatted : enable

out gl_PerVertex
{
    vec4 gl_Position;
};

layout ( location = 0 ) flat out uint output_instance_id;

void main() {
  gl_Position = vec4( 0.0, 0.0, 0.0, 1.0 );
  output_instance_id = gl_InstanceIndex;
}

