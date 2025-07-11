#ifndef GCT_SHADER_PBD_COLLISION_CONSTRAINT_H
#define GCT_SHADER_PBD_COLLISION_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/constraint.h>

vec4 pbd_collision_constraint_dx(
  uint particle_id,
  uint particle_offset,
  uint constraint_offset,
  uint constraint_index
) {
  const uint iter = constraint_begin( particle_id, constraint_offset );
  const bool is_end = constraint_is_end( iter + constraint_index );
  const constraint_type dc =
    is_end ?
    constraint_type( 0, 0.0 ) :
    constraint_get( iter + constraint_index );
  const vec3 p0 = particle_pool[ particle_offset + particle_id ].position;
  const vec3 pp0 = particle_pool[ particle_offset + particle_id ].previous_position;
  const vec3 p1 = particle_pool[ particle_offset + ( is_end ? particle_id : dc.to_id ) ].position;
  vec3 n1 = particle_pool[ particle_offset + ( is_end ? particle_id : dc.to_id ) ].normal;
  const bool front = dot( ( pp0 - p1 ), n1 ) >= 0.0;
  n1 = front ? n1 : -n1;
  const float thickness = 0.01;
  
  const float intrusion_depth = max( -( dot( ( p0 - p1 ), n1 ) - thickness ), 0.0 );
  
  const float dx_length = intrusion_depth;
  const vec3 dx = is_end ? vec3( 0, 0, 0 ) : normalize( n1 ) * dx_length;
  return subgroupAdd( vec4( dx, ( dx == vec3( 0, 0, 0 ) ) ? 0.0 : 1.0 ) );
}

#endif

