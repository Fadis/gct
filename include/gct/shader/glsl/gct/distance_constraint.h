#ifndef GCT_SHADER_DISTANCE_CONSTRAINT_H
#define GCT_SHADER_DISTANCE_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>


uint distance_constraint_begin(
  uint particle_id,
  uint id_offset
) {
  return id_offset + particle_id * 32;
}

uint distance_constraint_next(
  uint gcid
) {
  return gcid + 1;
}

bool distance_constraint_is_end(
  uint gcid
) {
  return distance_constraint_pool[ gcid ].to_id == 0;
}

distance_constraint_type distance_constraint_get(
  uint gcid
) {
  distance_constraint_type v = distance_constraint_pool[ gcid ];
  v.to_id--;
  return v;
}

void distance_constraint_insert_unidirectional(
  uint from_id,
  uint to_id,
  uint id_offset,
  float stiffness,
  float natural_distance
) {
  uint gcid = distance_constraint_begin( from_id, id_offset );
  for( uint cid = 0; cid < 31; cid++ ) {
    const uint orig = atomicCompSwap(
      distance_constraint_pool[ gcid ].to_id,
      0,
      to_id + 1
    );
    if( orig == to_id + 1 ) break;
    if( orig == 0 ) {
      distance_constraint_pool[ gcid ].stiffness = stiffness;
      distance_constraint_pool[ gcid ].natural_distance = natural_distance;
      break;
    }
    gcid = distance_constraint_next( gcid );
  }
}
void distance_constraint_insert(
  uint from_id,
  uint to_id,
  uint id_offset,
  float stiffness,
  float natural_distance
) {
  distance_constraint_insert_unidirectional( from_id, to_id, id_offset, stiffness, natural_distance );
  distance_constraint_insert_unidirectional( to_id, from_id, id_offset, stiffness, natural_distance );
}

void distance_constraint_insert(
  uint i0,
  uint i1,
  uint i2,
  vec3 p0,
  vec3 p1,
  vec3 p2,
  uint id_offset,
  float stiffness
) {
  distance_constraint_insert( i0, i1, id_offset, stiffness, distance( p0, p1 ) );
  distance_constraint_insert( i1, i2, id_offset, stiffness, distance( p1, p2 ) );
  distance_constraint_insert( i2, i0, id_offset, stiffness, distance( p2, p0 ) );
}

void distance_constraint_insert(
  mat4 l2w,
  uint accessor_id,
  uint input_primitive_id,
  uint id_offset,
  float stiffness
) {
  const uint i0 = read_index( accessor_pool[ accessor_id + 0 ], input_primitive_id * 3u + 0u );
  const uint i1 = read_index( accessor_pool[ accessor_id + 0 ], input_primitive_id * 3u + 1u );
  const uint i2 = read_index( accessor_pool[ accessor_id + 0 ], input_primitive_id * 3u + 2u );
  
  const vec4 p0 = l2w * read_vertex( accessor_pool[ accessor_id + 1 ], i0, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  const vec4 p1 = l2w * read_vertex( accessor_pool[ accessor_id + 1 ], i1, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  const vec4 p2 = l2w * read_vertex( accessor_pool[ accessor_id + 1 ], i2, vec4( 0.0, 0.0, 0.0, 1.0 ) );

  distance_constraint_insert( i0, i1, i2, p0.xyz/p0.w, p1.xyz/p1.w, p2.xyz/p2.w, id_offset, stiffness );
}

#endif

