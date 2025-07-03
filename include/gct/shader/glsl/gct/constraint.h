#ifndef GCT_SHADER_CONSTRAINT_H
#define GCT_SHADER_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>


uint constraint_begin(
  uint particle_id,
  uint id_offset
) {
  return id_offset + particle_id * 32;
}

uint constraint_next(
  uint gcid
) {
  return gcid + 1;
}

bool constraint_is_end(
  uint gcid
) {
  return constraint_pool[ gcid ].to_id == 0;
}

constraint_type constraint_get(
  uint gcid
) {
  constraint_type v = constraint_pool[ gcid ];
  v.to_id--;
  return v;
}

void constraint_set_lambda(
  uint gcid,
  float lambda
) {
  constraint_pool[ gcid ].lambda = lambda;
}

void constraint_insert_unidirectional(
  uint from_id,
  uint to_id,
  uint id_offset
) {
  uint hoge = 0;
  uint gcid = constraint_begin( from_id, id_offset );
  for( uint cid = 0; cid < 31; cid++ ) {
    const uint orig = atomicCompSwap(
      constraint_pool[ gcid ].to_id,
      0,
      to_id + 1
    );
    if( orig == to_id + 1 ) break;
    if( orig == 0 ) break;
    gcid++;
    //constraint_pool[ 0 ].to_id = 0;
    //gcid = constraint_next( gcid );
  }
}
void constraint_insert(
  uint from_id,
  uint to_id,
  uint id_offset
) {
  constraint_insert_unidirectional( from_id, to_id, id_offset );
  constraint_insert_unidirectional( to_id, from_id, id_offset );
}

#endif


