#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_shader_16bit_storage : enable

flat layout (location = 0) in vec4 input_color;
flat layout (location = 1) in uvec2 input_semantic;
flat layout (location = 2) in uint input_texid;
layout (location = 3) in vec2 input_texcoord;
layout (location = 0) out vec4 output_color;
layout (location = 1) out vec2 output_semantic;
layout(binding = 1) uniform sampler2D tex[ 16 ];

vec3 gamma( vec3 v ) {
  return min( max( v / (v + 0.155 ) * 1.019, vec3( 0, 0, 0 ) ), vec3( 1, 1, 1 ) );
}

void main()  {
  if( input_texid == 0 ) {
    output_color = vec4( gamma( input_color.rgb ), input_color.a );
  }
  else {
    vec4 sampled = texture( tex[ input_texid - 1 ], input_texcoord );
    output_color = vec4( gamma( sampled.rgb ), sampled.a );
  }
  output_semantic = input_semantic;
}

