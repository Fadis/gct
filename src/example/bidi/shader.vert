#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_shader_16bit_storage : enable

struct rect_info {
  vec4 color;
  u16vec2 offset;
  u16vec2 extent;
  u16vec2 semantic;
  uint16_t texid;
  uint16_t depth;
};
layout(std430,binding = 0) buffer Uniforms {
  mat4 world_matrix;
  rect_info rects[ 65536 ];
  float scale_factor;
} uniforms;

layout (location = 0) in vec3 input_position;
flat layout (location = 0) out vec4 output_color;
flat layout (location = 1) out uvec2 output_semantic;
flat layout (location = 2) out uint output_texid;
layout (location = 3) out vec2 output_texcoord;
out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 pixel_pos = vec4(
    (input_position.x * float( uint( uniforms.rects[ gl_InstanceIndex ].extent.x ) ) +
    float( uint( uniforms.rects[ gl_InstanceIndex ].offset.x ) ) ) * uniforms.scale_factor,
    (input_position.y * float( uint( uniforms.rects[ gl_InstanceIndex ].extent.y ) ) +
    float( uint( uniforms.rects[ gl_InstanceIndex ].offset.y ) ) ) * uniforms.scale_factor,
    float( 65535 - uint( uniforms.rects[ gl_InstanceIndex ].depth ) ) / 65536.f, 1.0
  );
  gl_Position = uniforms.world_matrix * pixel_pos;
  output_color = vec4( uniforms.rects[ gl_InstanceIndex ].color.r, uniforms.rects[ gl_InstanceIndex ].color.g, uniforms.rects[ gl_InstanceIndex ].color.b, 1.0 ); //uniforms.rects[ gl_InstanceIndex ].color;
  output_semantic = uvec2( uniforms.rects[ gl_InstanceIndex ].semantic );
  output_texid = uint( uniforms.rects[ gl_InstanceIndex ].texid );
  output_texcoord = vec2( input_position.x, 1.0 - input_position.y );
}

