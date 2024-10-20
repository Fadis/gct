#version 450

#extension GL_ARB_separate_shader_objects : enable
#extension GL_ARB_shading_language_420pack : enable
#extension GL_KHR_shader_subgroup_basic : enable
#extension GL_KHR_shader_subgroup_arithmetic : enable
#extension GL_KHR_memory_scope_semantics : enable
#extension GL_EXT_scalar_block_layout : enable

layout (points) in;
layout (triangle_strip,max_vertices=216) out;

#include "scene_graph.h"
#include "global_uniforms.h"

layout (set = 1, binding = 19, rgba32f) uniform image2DArray gbuffer;

in gl_PerVertex
{
    vec4 gl_Position;
} gl_in[];


out gl_PerVertex
{
    vec4 gl_Position;
};

layout (location = 0) in vec2 input_light0[];
layout (location = 1) in vec2 input_light1[];

layout (location = 0) out vec2 output_texcoord;
layout (location = 1) out vec4 output_energy;

void main() {
}

