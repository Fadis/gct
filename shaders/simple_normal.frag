#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 0) out vec4 output_color;


vec3 light_pos = vec3( 2, -2, 2 );

vec3 eotf( vec3 v ) {
  return min( max( v / (v + 0.155 ) * 1.019, vec3( 0, 0, 0 ) ), vec3( 1, 1, 1 ) );
}

void main()  {
  float light = max( dot( normalize( light_pos ), input_normal ), 0.0 );
  output_color = vec4( eotf( vec3( light, light, light ) ), 1.0 );
}


