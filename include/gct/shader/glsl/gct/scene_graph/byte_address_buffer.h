#ifndef GCT_SHADER_SCENE_GRAPH_BYTE_ADDRESS_BUFFER_H
#define GCT_SHADER_SCENE_GRAPH_BYTE_ADDRESS_BUFFER_H
#include <gct/scene_graph/vertex_buffer_pool.h>


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

