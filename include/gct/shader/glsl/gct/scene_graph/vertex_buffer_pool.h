#ifndef GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_POOL_H
#define GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_POOL_H

#ifndef GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID
#define GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID 4
#endif

#ifdef GCT_ENABLE_8BIT_16BIT_STORAGE
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferI8 {
  int8_t data[];
} vertex_buffer_i8[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferU8 {
  uint8_t data[];
} vertex_buffer_u8[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferI16 {
  int16_t data[];
} vertex_buffer_i16[];
layout (set = GCT_SHADER_SCENE_GRAPH_VERTEX_BUFFER_SET_ID, binding = 0, std430) buffer VertexBufferU16 {
  uint16_t data[];
} vertex_buffer_u16[];
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

#endif

