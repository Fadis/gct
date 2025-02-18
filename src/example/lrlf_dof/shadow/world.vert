#version 460

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable

layout (location = 0) in vec3 input_position;
layout (location = 0) out vec4 output_position;

#include <gct/scene_graph.h>

out gl_PerVertex
{
    vec4 gl_Position;
};

void main() {
  vec4 local_pos = vec4( input_position.xyz, 1.0 );
  mat4 world_matrix = matrix_pool[ instance_resource_index[ push_constants.instance ].world_matrix ];
  vec4 pos = world_matrix * local_pos;
  gl_Position = pos;
  output_position = pos;
}

