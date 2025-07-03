#ifndef GCT_SHADER_VERTEX_TO_PRIMITIVE_H
#define GCT_SHADER_VERTEX_TO_PRIMITIVE_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>

uint vertex_to_primitive_begin(
  uint vertex_id,
  uint id_offset
) {
  return id_offset + vertex_id * 32;
}

uint vertex_to_primitive_next(
  uint gvid
) {
  return gvid + 1;
}

bool vertex_to_primitive_is_end(
  uint gvid
) {
  return vertex_to_primitive_pool[ gvid ] == 0;
}

uint vertex_to_primitive_get(
  uint gvid
) {
  return vertex_to_primitive_pool[ gvid ] - 1;
}

void vertex_to_primitive_insert_internal(
  uint vertex_id,
  uint primitive_id,
  uint id_offset
) {
  uint gvid = vertex_to_primitive_begin( vertex_id, id_offset );
  for( uint vid = 0; vid < 31; vid++ ) {
    const uint orig = atomicCompSwap(
      vertex_to_primitive_pool[ gvid ],
      0,
      primitive_id + 1
    );
    if( orig == primitive_id + 1 ) break;
    if( orig == 0 ) break;
    gvid = vertex_to_primitive_next( gvid );
  }
}

void vertex_to_primitive_insert(
  uint accessor_id,
  uint primitive_id,
  uint id_offset
) {
  const uint i0 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 0u );
  const uint i1 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 1u );
  const uint i2 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 2u );
  vertex_to_primitive_insert_internal( i0, primitive_id, id_offset );
  vertex_to_primitive_insert_internal( i1, primitive_id, id_offset );
  vertex_to_primitive_insert_internal( i2, primitive_id, id_offset );
}

#endif

