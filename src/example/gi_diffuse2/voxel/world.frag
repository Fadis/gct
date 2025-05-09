#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include <gct/encode_pos.h>

layout (location = 0) in vec4 input_position;

layout (set=1, binding = 3, r32ui) uniform uimage3D dest_image;

void main()  {
  const ivec3 image_size = imageSize( dest_image );
  //const vec3 world_pos = vec3( ( input_position.xy + 1.0 )/ 2.0 * 256.0, input_position.z * 256.0 );
  const ivec3 world_pos = ivec3( ( input_position.xyz ) * ( image_size.xyz - vec3( 1, 1, 1 ) ) );
  imageStore( dest_image, world_pos, uvec4( encode_pos_3d( world_pos ), 0, 0, 0 ) );
}

