#ifndef GCT_SHADER_PBD_DISTANCE_CONSTRAINT_H
#define GCT_SHADER_PBD_DISTANCE_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/distance_constraint.h>

vec4 pbd_distance_constraint_dx(
  uint particle_id,
  uint particle_offset,
  uint distance_constraint_offset,
  uint constraint_index
) {
  const uint iter = distance_constraint_begin( particle_id, distance_constraint_offset );
  const bool is_end = distance_constraint_is_end( iter + constraint_index );
  const distance_constraint_type dc =
    is_end ?
    distance_constraint_type( 0, 0.0, 0.0, 0.0 ) :
    distance_constraint_get( iter + constraint_index );
  const vec3 p0 = particle_pool[ particle_offset + particle_id ].position.xyz;
  const vec3 p1 = particle_pool[ particle_offset + ( is_end ? particle_id : dc.to_id ) ].position.xyz;
  const float d = distance( p0, p1 );
  const float dx_length = -dc.stiffness * ( d - dc.natural_distance );
  const vec3 dx = is_end ? vec3( 0, 0, 0 ) : normalize( p0 - p1 ) * dx_length;
  return subgroupAdd( vec4( dx, is_end ? 0.0 : 1.0 ) );
}

#endif

