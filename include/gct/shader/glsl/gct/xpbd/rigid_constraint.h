#ifndef GCT_SHADER_PBD_RIGID_CONSTRAINT_H
#define GCT_SHADER_PBD_RIGID_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/constraint.h>
#include <gct/lambda.h>
#include <gct/quaternion.h>
#include <gct/aabb_type.h>
struct rigid_collision_dx_dq {
  vec3 dx;
  vec4 dq;
  uint dx_count;
  uint dq_count;
};

rigid_collision_dx_dq rigid_collision_constraint_dx(
  uint constraint_offset,
  uint constraint_index,
  float dt
) {
  const uint iter = rigid_collision_constraint_next(
    rigid_collision_constraint_begin( constraint_offset ),
    constraint_index
  );
  bool is_end = rigid_collision_constraint_is_end( iter );

  const uvec2 dc =
    is_end ? uvec2( 0, 0 ) :
    uvec2(
      rigid_collision_constraint_get_from( iter + constraint_index ),
      rigid_collision_constraint_get_to( iter + constraint_index )
    );

  const uint rigid0 = particle_pool[ dc.x ].rigid;
  const vec3 p0 = particle_pool[ dc.x ].position;
  const vec3 p_prev0 = particle_pool[ dc.x ].previous_position;
  const vec4 q0 = matrix_pool[ rigid_pool[ rigid0 ].trs ][ 1 ];
  const vec3 dir0 = p0 - p_prev0;
  const vec3 n0 = particle_pool[ dc.x ].normal;
  const vec3 r0 = particle_pool[ dc.x ].local_r;
  const float inv_m0 = 1.0/rigid_pool[ rigid0 ].mass;
  const mat3 inv_i0 = mat3( matrix_pool[ rigid_pool[ rigid0 ].inversed_momentum_inertia_tensor ] );

  const uint rigid1 = particle_pool[ dc.y ].rigid;
  const vec3 p1 = particle_pool[ dc.y ].position;
  const vec3 p_prev1 = particle_pool[ dc.y ].previous_position;
  const vec3 dir1 = p1 - p_prev1;
  const vec3 n1 = particle_pool[ dc.y ].normal;
  const vec3 r1 = particle_pool[ dc.y ].local_r;
  const float inv_m1 = 1.0/rigid_pool[ rigid1 ].mass;
  const mat3 inv_i1 = mat3( matrix_pool[ rigid_pool[ rigid1 ].inversed_momentum_inertia_tensor ] );

  if( dot( n0, n1 ) >= 0.0 ) {
    is_end == true;
  }

  const float c = distance( p0, p1 );

  if( c < 0.000001 )  {
    is_end = true;
  }

  const vec3 n = n1;// normalize( p0 - p1 );

  const float w0 = inv_m0 + dot( cross( r0, n1 ), inv_i0 * cross( r0, n1 ) );
  
  const float w1 = inv_m1 + dot( cross( r1, n0 ), inv_i1 * cross( r1, n0 ) );
  
  const float alpha = 0.000005;
  const float alpha_dt2 = alpha / ( dt * dt );
  
  const float dl = ( -c ) / ( w0 + w1 + alpha_dt2 );

  const float r = abs( dot( n, normalize( r0 ) ) );

  vec3 p = n * dl;
  
  const float dlf = -0.02;//( -c ) / ( w0 + w1 );
  const vec3 delta_p = ( p0 - p_prev0 ) - ( p1 - p_prev1 );
  const vec3 delta_pt = delta_p - dot( delta_p, n ) * n;

  vec3 dx = r * p * inv_m0 +delta_pt * dlf;
  vec4 dq =
    1.0/2.0 * quaternion_quaternion_mult( vec4( inv_i0 * cross( r0, sqrt( 1.0 - r * r ) * p ), 0.0 ), q0 ) +
    1.0/2.0 * quaternion_quaternion_mult( vec4( inv_i0 * cross( r0, delta_pt * dlf ), 0.0 ), q0 );

  dx = is_end ? vec3( 0, 0, 0 ) : dx;
  dq = is_end ? vec4( 0, 0, 0, 0 ) : dq;

  return rigid_collision_dx_dq(
    dx,
    dq,
    dx == vec3( 0, 0, 0 ) ? 0u : 2u,
    dq == vec4( 0, 0, 0, 0 ) ? 0u : 2u
  );
}


rigid_collision_dx_dq rigid_border_solve(
  vec3 p0,
  vec3 p_prev0,
  vec4 q0,
  vec3 n0,
  vec3 r0,
  float inv_m0,
  mat3 inv_i0,
  vec3 p1,
  float dt
) {
  
  const vec3 dir0 = p0 - p_prev0;

  if( dot( n0, p0 - p1 ) >= 0.0 ) {
    return rigid_collision_dx_dq(
      vec3( 0, 0, 0 ),
      vec4( 0, 0, 0, 0 ),
      0,
      0
    );
  }
  if( dot( dir0, p1 - p0 ) >= 0.0 ) {
    return rigid_collision_dx_dq(
      vec3( 0, 0, 0 ),
      vec4( 0, 0, 0, 0 ),
      0,
      0
    );
  }
  if( dot( -dir0, p0 - p1 ) >= 0.0 ) {
    return rigid_collision_dx_dq(
      vec3( 0, 0, 0 ),
      vec4( 0, 0, 0, 0 ),
      0,
      0
    );
  }
  const float c = distance( p0, p1 );
  const vec3 n = ( p0 - p1 )/c;

  const float w0 = inv_m0 + dot( cross( r0, n ), inv_i0 * cross( r0, n ) );
 
  const float alpha = 0.0;//0.000005;
  const float alpha_dt2 = alpha / ( dt * dt );
  
  const float dl = ( -c ) / ( w0 + alpha_dt2 );

  const float r = abs( dot( n, normalize( r0 ) ) );

  vec3 p = n * dl;

  const float dlf = -0.02;//( -c ) / ( w0 );
  
  const vec3 delta_p = ( p0 - p_prev0 );
  const vec3 delta_pt = ( delta_p - dot( delta_p, n ) * n );

  vec3 dx = r * p * inv_m0 +delta_pt * dlf /* * inv_m0*/;
  vec4 dq =
    1.0/2.0 * quaternion_quaternion_mult( vec4( inv_i0 * cross( r0, sqrt( 1.0 - r * r ) * p ), 0.0 ), q0 ) +
    1.0/2.0 * quaternion_quaternion_mult( vec4( inv_i0 * cross( r0, delta_pt * dlf ), 0.0 ), q0 );

  //dx *= 0.01;
  //dq *= 0.01;

  vec3 dx_sum = subgroupAdd( dx );
  uint dx_count = subgroupAdd( dx == vec3( 0, 0, 0 ) ? 0u : 2u );
  vec4 dq_sum = subgroupAdd( dq );
  uint dq_count = subgroupAdd( dq == vec4( 0, 0, 0, 0 ) ? 0u : 2u );

  return rigid_collision_dx_dq(
    dx_sum,
    dq_sum,
    dx_count,
    dq_count
  );
}

rigid_collision_dx_dq rigid_border_dx(
  uint rigid_id,
  uint global_particle_id,
  aabb_type aabb,
  float dt
) {
  vec3 dx = vec3( 0.0, 0.0, 0.0 );
  vec4 dq = vec4( 0.0, 0.0, 0.0, 0.0 );
  uint dx_count = 0u;
  uint dq_count = 0u;
  const mat3 inv_i0 = mat3( matrix_pool[ rigid_pool[ rigid_id ].inversed_momentum_inertia_tensor ] );
  const float inv_m0 = 1.0/rigid_pool[ rigid_id ].mass;
  
  const vec3 p0 = particle_pool[ global_particle_id ].position;
  const vec3 p_prev0 = particle_pool[ global_particle_id ].previous_position;
  const vec4 q0 = matrix_pool[ rigid_pool[ rigid_id ].trs ][ 1 ];
  const vec3 n0 = particle_pool[ global_particle_id ].normal;
  const vec3 r0 = particle_pool[ global_particle_id ].local_r;
  if( p0.x > aabb.max.x ) {
    const vec3 p1 = vec3( aabb.max.x, p0.y, p0.z );
    rigid_collision_dx_dq d = rigid_border_solve( p0, p_prev0, q0, n0, r0, inv_m0, inv_i0, p1, dt );
    dx += d.dx;
    dq += d.dq, dq;
    dx_count += d.dx_count;
    dq_count += d.dq_count;
  }
  else if( p0.x < aabb.min.x ) {
    const vec3 p1 = vec3( aabb.min.x, p0.y, p0.z );
    rigid_collision_dx_dq d = rigid_border_solve( p0, p_prev0, q0, n0, r0, inv_m0, inv_i0, p1, dt );
    dx += d.dx;
    dq += d.dq, dq;
    dx_count += d.dx_count;
    dq_count += d.dq_count;
  }
  if( p0.y > aabb.max.y ) {
    const vec3 p1 = vec3( p0.x, aabb.max.y, p0.z );
    rigid_collision_dx_dq d = rigid_border_solve( p0, p_prev0, q0, n0, r0, inv_m0, inv_i0, p1, dt );
    dx += d.dx;
    dq += d.dq;
    dx_count += d.dx_count;
    dq_count += d.dq_count;
  }
  else if( p0.y < aabb.min.y ) {
    const vec3 p1 = vec3( p0.x, aabb.min.y, p0.z );
    rigid_collision_dx_dq d = rigid_border_solve( p0, p_prev0, q0, n0, r0, inv_m0, inv_i0, p1, dt );
    dx += d.dx;
    dq += d.dq, dq;
    dx_count += d.dx_count;
    dq_count += d.dq_count;
  }
  if( p0.z > aabb.max.z ) {
    const vec3 p1 = vec3( p0.x, p0.y, aabb.max.z );
    rigid_collision_dx_dq d = rigid_border_solve( p0, p_prev0, q0, n0, r0, inv_m0, inv_i0, p1, dt );
    dx += d.dx;
    dq += d.dq;
    dx_count += d.dx_count;
    dq_count += d.dq_count;
  }
  else if( p0.z < aabb.min.z ) {
    const vec3 p1 = vec3( p0.x, p0.y, aabb.min.z );
    rigid_collision_dx_dq d = rigid_border_solve( p0, p_prev0, q0, n0, r0, inv_m0, inv_i0, p1, dt );
    dx += d.dx;
    dq += d.dq, dq;
    dx_count += d.dx_count;
    dq_count += d.dq_count;
  }

  return rigid_collision_dx_dq(
    dx,
    dq,
    dx_count,
    dq_count
  );
}

#endif

