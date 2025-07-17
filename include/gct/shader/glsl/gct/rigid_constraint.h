#ifndef GCT_SHADER_RIGID_CONSTRAINT_H
#define GCT_SHADER_RIGID_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>


uint rigid_constraint_begin(
  uint constraint_offset
) {
  return constraint_offset;
}

uint rigid_constraint_next(
  uint gcid
) {
  return gcid + 2;
}

bool rigid_constraint_is_end(
  uint gcid
) {
  return constraint_pool[ gcid ] == 0;
}

uvec2 rigid_constraint_get(
  uint gcid
) {
  uint from = constraint_pool[ gcid + 1 ];
  uint to = constraint_pool[ gcid ];
  from--;
  to--;
  return uvec2( from, to );
}

float rigid_constraint_get_lambda(
  uint gcid
) {
  return lambda_pool[ gcid ];
}

void rigid_constraint_set_lambda(
  uint gcid,
  float v
) {
  lambda_pool[ gcid ] = v;
}


void rigid_constraint_insert_unidirectional(
  uint from_particle_id,
  uint to_particle_id,
  uint constraint_offset
) {
  uint hoge = 0;
  uint gcid = rigid_constraint_begin( constraint_offset );
  for( uint cid = 0; cid < 256; cid++ ) {
    const uint orig = atomicCompSwap(
      constraint_pool[ gcid ],
      0,
      to_particle_id + 1
    );
    if( orig == to_particle_id + 1 ) {
      uint existing = constraint_pool[ gcid + 1 ] - 1;
      if( existing == from_particle_id ) {
        break;
      }
    }
    else if( orig == 0 ) {
      constraint_pool[ gcid + 1 ] = from_particle_id + 1;
      break;
    }
    gcid = rigid_constraint_next( gcid );
  }
}



#endif


