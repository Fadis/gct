#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_EXT_nonuniform_qualifier : enable

#include <gct/global_uniforms.h>
#include <gct/scene_graph.h>

layout(early_fragment_tests) in;

layout(binding = 6,set=1,std430) buffer Condition {
  uint condition[];
};

layout(early_fragment_tests) in;

layout ( location = 0 ) flat in uint input_instance_id;

void main()  {
  condition[ instance_resource_index[ resource_pair[ input_instance_id ].inst ].visibility ] = 1;
}

