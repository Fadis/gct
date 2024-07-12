#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_position;

layout (set=1, binding = 2, r32ui) uniform uimage3D voxel;

void main()  {
  const ivec3 image_size = imageSize( voxel );
  const ivec3 world_pos = ivec3( ( input_position.xyz ) * ( image_size.xyz - vec3( 1, 1, 1 ) ) );
  imageStore( voxel, world_pos, uvec4( ( world_pos.x ) << 20 | ( world_pos.y << 10 ) | world_pos.z, 0, 0, 0 ) );
}

