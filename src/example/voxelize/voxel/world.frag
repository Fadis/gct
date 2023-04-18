#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_position;

layout (set=1, binding = 2, r32ui) uniform uimage3D dest_image;

void main()  {
  const ivec3 image_size = imageSize( dest_image );
  //const vec3 world_pos = vec3( ( input_position.xy + 1.0 )/ 2.0 * 256.0, input_position.z * 256.0 );
  const ivec3 world_pos = ivec3( ( input_position.xyz + 1.0 )/2.0 * ( image_size.xyz - vec3( 1, 1, 1 ) ) );
  imageStore( dest_image, world_pos, uvec4( ( world_pos.x ) << 20 | ( world_pos.y << 10 ) | world_pos.z, 0, 0, 0 ) );
}

