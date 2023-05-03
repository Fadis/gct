#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec4 input_position;

layout (set=1, binding = 2, r8) uniform image3D l0_image;
layout (set=1, binding = 3, r8) uniform image3D l1_image;
layout (set=1, binding = 4, r8) uniform image3D l2_image;
layout (set=1, binding = 5, r8) uniform image3D l3_image;

void main()  {
  const ivec3 l0_image_size = imageSize( l0_image );
  const ivec3 l1_image_size = imageSize( l1_image );
  const ivec3 l2_image_size = imageSize( l2_image );
  const ivec3 l3_image_size = imageSize( l3_image );
  const ivec3 l0_world_pos = ivec3( ( input_position.xyz ) * ( l0_image_size.xyz - vec3( 1, 1, 1 ) ) );
  const ivec3 l1_world_pos = ivec3( ( input_position.xyz ) * ( l1_image_size.xyz - vec3( 1, 1, 1 ) ) );
  const ivec3 l2_world_pos = ivec3( ( input_position.xyz ) * ( l2_image_size.xyz - vec3( 1, 1, 1 ) ) );
  const ivec3 l3_world_pos = ivec3( ( input_position.xyz ) * ( l3_image_size.xyz - vec3( 1, 1, 1 ) ) );
  imageStore( l0_image, l0_world_pos, vec4( 1, 0, 0, 0 ) );
  imageStore( l1_image, l1_world_pos, vec4( 1, 0, 0, 0 ) );
  imageStore( l2_image, l2_world_pos, vec4( 1, 0, 0, 0 ) );
  imageStore( l3_image, l3_world_pos, vec4( 1, 0, 0, 0 ) );
}

