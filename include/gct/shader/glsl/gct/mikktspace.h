#ifndef GCT_SHADER_MIKKTSPACE_H
#define GCT_SHADER_MIKKTSPACE_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/vertex_to_primitive.h>

vec3 mikktspace(
  uint accessor_id,
  uint primitive_id
) {
  const uint i0 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 0u );
  const uint i1 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 1u );
  const uint i2 = read_index( accessor_pool[ accessor_id + 0 ], primitive_id * 3u + 2u );
  
  const vec3 p0 = read_vertex( accessor_pool[ accessor_id + 1 ], i0, vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz;
  const vec3 p1 = read_vertex( accessor_pool[ accessor_id + 1 ], i1, vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz;
  const vec3 p2 = read_vertex( accessor_pool[ accessor_id + 1 ], i2, vec4( 0.0, 0.0, 0.0, 1.0 ) ).xyz;
  
  const vec2 t0 = read_vertex( accessor_pool[ accessor_id + 4 ], i0, vec4( 0.0, 0.0, 0.0, 1.0 ) ).xy;
  const vec2 t1 = read_vertex( accessor_pool[ accessor_id + 4 ], i1, vec4( 0.0, 0.0, 0.0, 1.0 ) ).xy;
  const vec2 t2 = read_vertex( accessor_pool[ accessor_id + 4 ], i2, vec4( 0.0, 0.0, 0.0, 1.0 ) ).xy;

  const vec3 p10 = p1 - p0;
  const vec3 p20 = p2 - p0;
  const vec2 t10 = t1 - t0;
  const vec2 t20 = t2 - t0;

  const float det = 1.0 / ( t10.x * t20.y - t10.y * t20.x );
	const vec3 t = det * ( p10 * t20.y - p20 * t10.y );
  return normalize( t );
}

vec3 mikktspace(
  uint accessor_id,
  uint vertex_id,
  uint id_offset
) {
  const uint iter = vertex_to_primitive_begin( vertex_id, id_offset );
  const uint primitive_id = vertex_to_primitive_get( iter );
  return mikktspace( accessor_id, primitive_id );
}

#endif

