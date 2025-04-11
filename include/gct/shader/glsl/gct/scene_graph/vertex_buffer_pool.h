#ifndef GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_POOL_H
#define GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_POOL_H

#ifndef GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID
#define GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID 4
#endif

layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferI32 {
  int data[];
} vertex_buffer_i32[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferU32 {
  uint data[];
} vertex_buffer_u32[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferF32 {
  float data[];
} vertex_buffer_f32[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferI32V2 {
  ivec2 data[];
} vertex_buffer_i32v2[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferU32V2 {
  uvec2 data[];
} vertex_buffer_u32v2[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferF32V2 {
  vec2 data[];
} vertex_buffer_f32v2[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferI32V4 {
  ivec4 data[];
} vertex_buffer_i32v4[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferU32V4 {
  uvec4 data[];
} vertex_buffer_u32v4[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferF32V4 {
  vec4 data[];
} vertex_buffer_f32v4[];

#endif

