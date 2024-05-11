#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable

#include "scene_graph.h"
#include "global_uniforms.h"

layout (location = 0) in vec3 input_position;
layout (location = 1) in vec3 input_normal;
layout (location = 3) in vec2 input_texcoord0;

layout (location = 0) out vec4 output_position;
layout (location = 1) out vec3 output_normal;
layout (location = 3) out vec2 output_tex_coord;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  mat4 world_matrix = matrix_pool[ instance_resource_index[ push_constants.instance ].world_matrix ];
  vec4 pos = world_matrix * local_pos;
  output_position = pos;
  vec3 normal = normalize( ( mat3(world_matrix) * input_normal ) );
  output_normal = normal;
  output_tex_coord = input_texcoord0;
  gl_Position =
    global_uniforms.projection_matrix *
    global_uniforms.camera_matrix * pos;
}

