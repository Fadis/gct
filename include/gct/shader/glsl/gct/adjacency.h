#ifndef GCT_SHADER_ADJACENCY_H
#define GCT_SHADER_ADJACENCY_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/vertex_to_primitive.h>

void adjacency_insert(
  uint from_primitive_id,
  uint edge_id,
  uint to_primitive_id,
  uint adjacency_id_offset
) {
  adjacency_pool[ adjacency_id_offset + from_primitive_id * 3u + edge_id ] = to_primitive_id + 1u;
}

bool adjacency_has_adjacency(
  uint from_primitive_id,
  uint edge_id,
  uint adjacency_id_offset
) {
  return adjacency_pool[ adjacency_id_offset + from_primitive_id * 3u + edge_id ] != 0u;
}

uint adjacency_get(
  uint from_primitive_id,
  uint edge_id,
  uint adjacency_id_offset
) {
  return adjacency_pool[ adjacency_id_offset + from_primitive_id * 3u + edge_id ] - 1u;
}

void dummy_adjacency_generate(
  uint accessor_id_offset,
  uint primitive_id,
  uint v2p_id_offset,
  uint adjacency_id_offset
) {
  adjacency_insert( primitive_id, 0u, 1u, adjacency_id_offset );
}

void adjacency_generate(
  uint accessor_id_offset,
  uint primitive_id,
  uint v2p_id_offset,
  uint adjacency_id_offset
) {
  const uint i0 = read_index( accessor_pool[ accessor_id_offset + 0 ], primitive_id * 3u + 0u );
  const uint i1 = read_index( accessor_pool[ accessor_id_offset + 0 ], primitive_id * 3u + 1u );
  const uint i2 = read_index( accessor_pool[ accessor_id_offset + 0 ], primitive_id * 3u + 2u );
  uint i0_iter = vertex_to_primitive_begin( i0, v2p_id_offset );
  for( uint slot_id = 0u; slot_id != 32u; slot_id++ ) {
    if( !vertex_to_primitive_is_end( i0_iter ) ) {
      const uint candidate_id = vertex_to_primitive_get( i0_iter );
      const uint ci0 = read_index( accessor_pool[ accessor_id_offset + 0 ], candidate_id * 3u + 0u );
      const uint ci1 = read_index( accessor_pool[ accessor_id_offset + 0 ], candidate_id * 3u + 1u );
      const uint ci2 = read_index( accessor_pool[ accessor_id_offset + 0 ], candidate_id * 3u + 2u );
      if( ci0 == i1 ) {
        adjacency_insert( primitive_id, 0u, candidate_id, adjacency_id_offset );
      }
      else if( ci0 == i2 ) {
        adjacency_insert( primitive_id, 2u, candidate_id, adjacency_id_offset );
      }
      else if( ci1 == i1 ) {
        adjacency_insert( primitive_id, 0u, candidate_id, adjacency_id_offset );
      }
      else if( ci1 == i2 ) {
        adjacency_insert( primitive_id, 2u, candidate_id, adjacency_id_offset );
      }
      else if( ci2 == i1 ) {
        adjacency_insert( primitive_id, 0u, candidate_id, adjacency_id_offset );
      }
      else if( ci2 == i2 ) {
        adjacency_insert( primitive_id, 2u, candidate_id, adjacency_id_offset );
      }
    }
    i0_iter = vertex_to_primitive_next( i0_iter );
  }
  uint i1_iter = vertex_to_primitive_begin( i1, v2p_id_offset );
  for( uint slot_id = 0u; slot_id != 32u; slot_id++ ) {
    if( !vertex_to_primitive_is_end( i1_iter ) ) {
      const uint candidate_id = vertex_to_primitive_get( i1_iter );
      const uint ci0 = read_index( accessor_pool[ accessor_id_offset + 0 ], candidate_id * 3u + 0u );
      const uint ci1 = read_index( accessor_pool[ accessor_id_offset + 0 ], candidate_id * 3u + 1u );
      const uint ci2 = read_index( accessor_pool[ accessor_id_offset + 0 ], candidate_id * 3u + 2u );
      if( ci0 == i2 ) {
        adjacency_insert( primitive_id, 1u, candidate_id, adjacency_id_offset );
      }
      else if( ci1 == i2 ) {
        adjacency_insert( primitive_id, 1u, candidate_id, adjacency_id_offset );
      }
      else if( ci2 == i2 ) {
        adjacency_insert( primitive_id, 1u, candidate_id, adjacency_id_offset );
      }
    }
    i1_iter = vertex_to_primitive_next( i1_iter );
  }
}

#endif

