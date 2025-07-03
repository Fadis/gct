#ifndef GCT_SCENE_GRAPH_SPATIAL_HASH_H
#define GCT_SCENE_GRAPH_SPATIAL_HASH_H

#include <gct/scene_graph/vertex_buffer_pool.h>

struct hash_table_type {
  uint descriptor_id;
  uint offset;
  uint size;
  float scale;
};

ivec3 spatial_hash_position_to_voxel(
  hash_table_type table,
  vec3 position
) {
  return ivec3( int( position.x * table.scale ), int( position.y * table.scale ), int( position.z * table.scale ) );
}

#define spatial_hash_iterator uint;

spatial_hash_iterator spatial_hash_function(
  hash_table_type table,
  ivec3 voxel
) {
  const spatial_hash_iterator h = uint( voxel.x * 92837111 ) ^ uint( voxel.y * 689287499 ) ^ uint( voxel.z * 283923481 );
  return table.offset + ( h % table.size );
}

spatial_hash_iterator spatial_hash_next(
  hash_table_type table,
  spatial_hash_iterator current
) {
  return table.offset + ( ( current - table.offset + 1u ) % table.size );
}

#define spatial_hash_entry ivec4;

spatial_hash_entry spatial_hash_read_table(
   hash_table_type table,
   spatial_hash_iterator current
) {
  return spatial_hash_entry(
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 0 ],
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 1 ],
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 2 ],
    int( vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 3 ] - 1 )
  );
}

bool spatial_hash_write_table(
   hash_table_type table,
   spatial_hash_iterator current,
   ivec3 key,
   uint value
) {
  const uint orig = atomicCompareSwap(
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 3 ],
    0u,
    value
  );
  if( orig == 0 ) {
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 0 ] = key.x;
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 1 ] = key.y;
    vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 2 ] = key.z;
  }
  return orig == 0;
}

bool spatial_hash_not_match(
   hash_table_type table,
   spatial_hash_iterator current,
   ivec3 voxel
) {
  const spatial_hash_entry key_value = spatial_hash_read_table( table, current );
  return key_value.w != 0 && voxel != key_value.xyz;
}

uint spatial_hash_next(
   hash_table_type table,
   spatial_hash_iterator current,
   ivec3 voxel
) {
  uint candidate = spatial_hash_next( table, current );
  while( spatial_hash_not_match( table, candidate, voxel ) ) {
    candidate = spatial_hash_next( table, candidate );
  }
  return candidate;
}

bool spatial_hash_is_end(
   hash_table_type table,
   spatial_hash_iterator current
) {
  return vertex_buffer_i32[ nonuniformEXT( table.descriptor_id ) ].data[ current * 4 + 3 ] == 0;
}

spatial_hash_iterator spatial_hash_find(
  hash_table_type table,
  ivec3 voxel
) {
  uint current = spatial_hash_function( table, voxel );
  if( spatial_hash_not_match( table, current, voxel ) ) {
    current = spatial_hash_next( table, current, voxel );
  }
  return current;
}

void spatial_hash_insert(
  hash_table_type table,
  ivec3 voxel,
  uint value
) {
  uint current = spatial_hash_find( table, voxel );
  while( spatial_hash_write_table( table, current, voxel, value ) == false ) {
    current = spatial_hash_next( table, current, voxel );
  }
}

void spatial_hash_insert_unique(
  hash_table_type table,
  ivec3 voxel,
  uint value
) {
  uint current = spatial_hash_find( table, voxel );
  while( spatial_hash_write_table( table, current, voxel, value ) == false ) {
    current = spatial_hash_next( table, current, voxel );
  }
}


#endif

