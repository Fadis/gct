#ifndef GCT_SHADER_SCENE_GRAPH_BYTE_ADDRESS_BUFFER_H
#define GCT_SHADER_SCENE_GRAPH_BYTE_ADDRESS_BUFFER_H
#include <gct/scene_graph/vertex_buffer_pool.h>

#ifdef GCT_ENABLE_8BIT_16BIT_STORAGE

uint16_t vertex_buffer_load_u16( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_u16[ vertex_buffer ].data[ ( offset >> 1 ) + 0 ];
}
int16_t vertex_buffer_load_i16( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_i16[ vertex_buffer ].data[ ( offset >> 1 ) + 0 ];
}
uint8_t vertex_buffer_load_u8( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_u8[ vertex_buffer ].data[ ( offset ) + 0 ];
}
int8_t vertex_buffer_load_i8( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_i8[ vertex_buffer ].data[ ( offset ) + 0 ];
}
float16_t vertex_buffer_load_f16( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_f16[ vertex_buffer ].data[ ( offset >> 1 ) + 0 ];
}

#endif

float vertex_buffer_load_f32( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_f32[ vertex_buffer ].data[ ( offset >> 2 ) + 0 ];
}

// ByteAddressBuffer互換load
uint vertex_buffer_load( uint vertex_buffer, uint offset ) {
  return
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 0 ];
}

uvec2 vertex_buffer_load2( uint vertex_buffer, uint offset ) {
  return uvec2(
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 0 ],
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 1 ]
  );
}

uvec3 vertex_buffer_load3( uint vertex_buffer, uint offset ) {
  return uvec3(
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 0 ],
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 1 ],
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 2 ]
  );
}

uvec4 vertex_buffer_load4( uint vertex_buffer, uint offset ) {
  return uvec4(
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 0 ],
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 1 ],
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 2 ],
    vertex_buffer_u32[ vertex_buffer ].data[ ( offset >> 2 ) + 3 ]
  );
}

#endif

