#ifndef GCT_SHADER_VERTEX_TO_PRIMITIVE_H
#define GCT_SHADER_VERTEX_TO_PRIMITIVE_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>

uint same_position_begin(
  uint vertex_id,
  uint id_offset
) {
  return id_offset + vertex_id * 32;
}

uint same_position_next(
  uint gvid
) {
  return gvid + 1;
}

bool same_position_is_end(
  uint gvid
) {
  return vertex_to_primitive_pool[ gvid ] == 0;
}

uint same_position_get(
  uint gvid
) {
  return vertex_to_primitive_pool[ gvid ] - 1;
}

void same_position_insert_internal(
  uint from_vertex_id,
  uint to_vertex_id,
  uint id_offset
) {
  uint gvid = same_position_begin( from_vertex_id, id_offset );
  for( uint vid = 0; vid < 31; vid++ ) {
    const uint orig = atomicCompSwap(
      vertex_to_primitive_pool[ gvid ],
      0,
      to_vertex_id + 1
    );
    if( orig == to_vertex_id + 1 ) break;
    if( orig == 0 ) break;
    gvid = same_position_next( gvid );
  }
}


void generate_same_position(
  uint accessor_id,
  uint from_vertex_id,
  uint to_vertex_id,
  uint id_offset
) {
  if( from_vertex_id >= to_vertex_id ) return;
  const vec4 p0 = read_vertex( accessor_pool[ accessor_id + 1 ], from_vertex_id, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  const vec4 p1 = read_vertex( accessor_pool[ accessor_id + 1 ], to_vertex_id, vec4( 0.0, 0.0, 0.0, 0.0 ) );
  if( p0 == p1 ) {
    same_position_insert_internal( from_vertex_id, to_vertex_id, id_offset );
    same_position_insert_internal( to_vertex_id, from_vertex_id, id_offset );
  }
}

bool same_position_equal(
  uint vertex_id0,
  uint vertex_id1,
  uint sp_id_offset
) {
  if( vertex_id0 == vertex_id1 ) return true;
  if( sp_id_offset == 0xFFFFFFFF ) return false;
  uint iter = same_position_begin( vertex_id0, sp_id_offset );
  bool equal = false;
  for( uint i = 0u; i != 32u; ++i ) {
    if( !same_position_is_end( iter ) ) {
      if( same_position_get( iter ) == vertex_id1 ) {
        equal = true;
      }
    }
    iter = same_position_next( iter );
  }
  return equal;
}

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
  uint v2p_id_offset,
  uint sp_id_offset
) {
  const uint i0 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 0u );
  const uint i1 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 1u );
  const uint i2 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 2u );
  vertex_to_primitive_insert_internal( i0, primitive_id, v2p_id_offset );
  vertex_to_primitive_insert_internal( i1, primitive_id, v2p_id_offset );
  vertex_to_primitive_insert_internal( i2, primitive_id, v2p_id_offset );
  if( sp_id_offset == 0xFFFFFFFF ) return;
  uint i0_iter = same_position_begin( i0, sp_id_offset );
  uint i1_iter = same_position_begin( i1, sp_id_offset );
  uint i2_iter = same_position_begin( i2, sp_id_offset );
  for( uint slot_id = 0u; slot_id != 32u; ++slot_id ) {
    if( !same_position_is_end( i0_iter ) ) {
      vertex_to_primitive_insert_internal( same_position_get( i0_iter ), primitive_id, v2p_id_offset );
    }
    i0_iter = same_position_next( i0_iter );
    if( !same_position_is_end( i1_iter ) ) {
      vertex_to_primitive_insert_internal( same_position_get( i1_iter ), primitive_id, v2p_id_offset );
    }
    i1_iter = same_position_next( i1_iter );
    if( !same_position_is_end( i2_iter ) ) {
      vertex_to_primitive_insert_internal( same_position_get( i2_iter ), primitive_id, v2p_id_offset );
    }
    i2_iter = same_position_next( i2_iter );
  }
}

#endif

