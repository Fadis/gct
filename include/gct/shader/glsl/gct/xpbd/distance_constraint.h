#ifndef GCT_SHADER_XPBD_DISTANCE_CONSTRAINT_H
#define GCT_SHADER_XPBD_DISTANCE_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/distance_constraint.h>


vec4 xpbd_distance_constraint_dx(
  uint particle_id,
  uint particle_offset,
  uint distance_constraint_offset,
  uint lambda_offset,
  uint constraint_index,
  float dt
) {
  const uint iter = distance_constraint_begin( particle_id, distance_constraint_offset );
  const uint lambda_iter = distance_constraint_begin( particle_id, lambda_offset );
  const bool is_end = distance_constraint_is_end( iter + constraint_index );
  const distance_constraint_type dc =
    is_end ?
    distance_constraint_type( 0, 0.0, 0.0, 0.0 ) :
    distance_constraint_get( iter + constraint_index );
  float lambda = distance_constraint_get_lambda( lambda_iter + constraint_index );

  //if( isnan( lambda ) || isinf( lambda ) ) lambda = 0.0;

  const vec3 p0 = particle_pool[ particle_offset + particle_id ].position.xyz;
  const vec3 p1 = particle_pool[ particle_offset + ( is_end ? particle_id : dc.to_id ) ].position.xyz;
  const float w0 = particle_pool[ particle_offset + particle_id ].w;
  const float w1 = particle_pool[ particle_offset + ( is_end ? particle_id : dc.to_id ) ].w;
  const float d = distance( p0, p1 );
  const float alpha = 1.0 / ( 6250.0 );// dc.stiffness;
  const float alpha_dt2 = alpha / ( dt * dt );

  const float c = d - dc.natural_distance;
  const vec3 grad_c = ( p0 - p1 ) / max( d, 0.01 );
  const float dl = ( -c - alpha_dt2 * lambda ) / ( w0 + w1 + alpha_dt2 );
  const vec3 dx = w0 * grad_c * dl;

  distance_constraint_set_lambda( lambda_iter + constraint_index, is_end ? 0.0 : lambda + dl );

  return subgroupAdd( vec4( dx, is_end ? 0.0 : 1.0 ) );
}

#endif

