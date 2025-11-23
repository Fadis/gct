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
  return id_offset + particle_id * 128;
}

uint constraint_next(
  uint gcid
) {
  return gcid + 1;
}

bool constraint_is_end(
  uint gcid
) {
  return constraint_pool[ gcid ] == 0;
}

uint constraint_get(
  uint gcid
) {
  uint v = constraint_pool[ gcid ];
  v--;
  return v;
}

void constraint_insert_unidirectional(
  uint from_id,
  uint to_id,
  uint id_offset
) {
  uint hoge = 0;
  uint gcid = constraint_begin( from_id, id_offset );
  for( uint cid = 0; cid < 128; cid++ ) {
    const uint orig = atomicCompSwap(
      constraint_pool[ gcid ],
      0,
      to_id + 1
    );
    if( orig == to_id + 1 ) break;
    if( orig == 0 ) break;
    gcid = constraint_next( gcid );
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

uint rigid_collision_constraint_begin(
  uint id_offset
) {
  return id_offset;
}

uint rigid_collision_constraint_next(
  uint gcid
) {
  return gcid + 2;
}

uint rigid_collision_constraint_next(
  uint gcid,
  uint count
) {
  return gcid + count * 2;
}


bool rigid_collision_constraint_is_end(
  uint gcid
) {
  return constraint_pool[ gcid ] == 0;
}

uint rigid_collision_constraint_get_from(
  uint gcid
) {
  uint v = constraint_pool[ gcid ];
  v--;
  return v;
}

uint rigid_collision_constraint_get_to(
  uint gcid
) {
  uint v = constraint_pool[ gcid + 1 ];
  v--;
  return v;
}

void rigid_collision_constraint_insert_unidirectional(
  uint from_id,
  uint to_id,
  uint id_offset
) {
  uint hoge = 0;
  uint gcid = rigid_collision_constraint_begin( id_offset );
  for( uint cid = 0; cid < 512; cid++ ) {
    const uint orig = atomicCompSwap(
      constraint_pool[ gcid ],
      0,
      from_id + 1
    );
    if( orig == 0 ) {
      constraint_pool[ gcid + 1 ] = to_id + 1;
      break;
    }
    gcid = rigid_collision_constraint_next( gcid );
  }
}

#endif


