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
layout (location = 4) out vec4 output_optflow;
layout (location = 5) flat out vec4 output_id;
layout (location = 6) out vec4 output_previous_position;

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  const vec4 local_pos = vec4( input_position.xyz, 1.0 );
  const mat4 world_matrix = matrix_pool[ instance_resource_index[ push_constants.instance ].world_matrix ];
  const vec4 world_pos = world_matrix * local_pos;
  vec4 screen_pos =
    matrix_pool[ global_uniforms.projection_matrix ] *
    matrix_pool[ global_uniforms.camera_matrix ] * world_pos;
  vec3 screen_pos_ = screen_pos.xyz / screen_pos.w;
  
  const mat4 previous_world_matrix = matrix_pool[ instance_resource_index[ push_constants.instance ].previous_world_matrix ];
  const vec4 previous_world_pos = previous_world_matrix * local_pos;
  vec4 previous_screen_pos =
    matrix_pool[ global_uniforms.previous_projection_matrix ] *
    matrix_pool[ global_uniforms.previous_camera_matrix ] * previous_world_pos;
  vec3 previous_screen_pos_ = previous_screen_pos.xyz / previous_screen_pos.w;

  output_optflow = 
    matrix_pool[ global_uniforms.projection_matrix ] *
    matrix_pool[ global_uniforms.camera_matrix ] *
    world_pos;

  output_position = world_pos;
  output_previous_position =
    matrix_pool[ global_uniforms.previous_projection_matrix ] *
    matrix_pool[ global_uniforms.previous_camera_matrix ] *
    previous_world_pos;
  vec3 normal = normalize( ( mat3(world_matrix) * input_normal ) );
  output_normal = normal;
  output_tex_coord = input_texcoord0;
  gl_Position = screen_pos;
  output_id = vec4( push_constants.instance, push_constants.primitive, gl_InstanceIndex, 0.0 );
}

