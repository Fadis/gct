#ifndef GCT_SHADER_SCENE_GRAPH_COUNTER_H
#define GCT_SHADER_SCENE_GRAPH_COUNTER_H
#include <gct/scene_graph/vertex_buffer_pool.h>

#ifdef GCT_ENABLE_8BIT_16BIT_STORAGE

#define GCT_COUNTER_INPUT_MESHLET_COUNT 0
#define GCT_COUNTER_OUTPUT_MESHLET_COUNT 1
#define GCT_COUNTER_INPUT_TRIANGLE_COUNT 2
#define GCT_COUNTER_OUTPUT_TRIANGLE_COUNT 3
#define GCT_COUNTER_SOFTRAST_TRIANGLE_COUNT 4
#define GCT_COUNTER_INVISIBLE_TRIANGLE_COUNT 5
#define GCT_COUNTER_FRAGMENT_SHADER_INVOCATION_COUNT 6
#define GCT_COUNTER_INPUT_PIXEL_COUNT 7
#define GCT_COUNTER_OUTPUT_PIXEL_COUNT 8

void increment_counter( uint vertex_buffer, uint index ) {
   atomicAdd( vertex_buffer_u32[ vertex_buffer ].data[ index ], 1u );
}

void add_counter( uint vertex_buffer, uint index, uint value ) {
   atomicAdd( vertex_buffer_u32[ vertex_buffer ].data[ index ], value );
}

#endif

#endif

