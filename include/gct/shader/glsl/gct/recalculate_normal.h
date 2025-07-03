#ifndef GCT_SHADER_RECALCULATE_NORMAL_H
#define GCT_SHADER_RECALCULATE_NORMAL_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/vertex_to_primitive.h>

vec3 calculate_local_normal_from_position(
  uint accessor_id,
  uint primitive_id
) {
  const uint i0 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 0u );
  const uint i1 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 1u );
  const uint i2 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 2u );
  
  const vec4 p0 = read_vertex( accessor_pool[ accessor_id + 1 ], i0, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  const vec4 p1 = read_vertex( accessor_pool[ accessor_id + 1 ], i1, vec4( 0.0, 0.0, 0.0, 1.0 ) );
  const vec4 p2 = read_vertex( accessor_pool[ accessor_id + 1 ], i2, vec4( 0.0, 0.0, 0.0, 1.0 ) );

   return normalize( cross( p1.xyz - p0.xyz, p2.xyz - p0.xyz ) );
}

vec3 calculate_smooth_local_normal(
  uint accessor_id,
  uint vertex_id,
  uint id_offset,
  uint index
) {
  const uint iter = vertex_to_primitive_begin( vertex_id, id_offset );
  const bool is_end = vertex_to_primitive_is_end( iter + index );
  const uint primitive_id = is_end ? 0u : vertex_to_primitive_get( iter + index );
  const vec3 n = is_end ? vec3( 0, 0, 0 ) : calculate_local_normal_from_position( accessor_id, primitive_id );
  const vec3 nsum = subgroupAdd( n );
  return normalize( nsum );
}

vec3 calculate_smooth_local_normal(
  uint accessor_id,
  uint vertex_id,
  uint id_offset
) {
  const uint iter = vertex_to_primitive_begin( vertex_id, id_offset );
  const bool is_end = vertex_to_primitive_is_end( iter );
  const uint primitive_id = is_end ? 0u : vertex_to_primitive_get( iter );
  const vec3 n = is_end ? vec3( 0, 0, 0 ) : calculate_local_normal_from_position( accessor_id, primitive_id );
  return n;
}


#endif

