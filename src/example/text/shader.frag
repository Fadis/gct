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

void main()  {
  if( input_texid == 0 ) {
    if( input_color.a == 0.0 ) {
      discard;
    }
    output_color = input_color;
  }
  else {
    vec4 sampled = texture( tex[ input_texid - 1 ], input_texcoord );
    if( sampled.a == 0.0 ) {
      discard;
    }
    output_color = sampled;
  }
  output_semantic = input_semantic;
}

