#ifndef GCT_SHADER_PBD_RIGID_CONSTRAINT_H
#define GCT_SHADER_PBD_RIGID_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/rigid_constraint.h>
#include <gct/lambda.h>
#include <gct/quaternion.h>
#include <gct/aabb_type.h>
struct rigid_collision_dx_dq {
  vec3 dx;
  vec4 dq;
  uint dx_count;
  uint dq_count;
};

// per constraint
rigid_collision_dx_dq rigid_collision_constraint_dx(
  uint rigid_id,
  uint particle_offset,
  uint constraint_offset,
  uint lambda_offset,
  uint constraint_index,
  float dt
) {
  const uint iter = rigid_constraint_begin( constraint_offset );
  const uint lambda_iter = rigid_lambda_begin( lambda_offset );
  const bool is_end = rigid_constraint_is_end( iter + constraint_index );
  const uvec2 dc =
    is_end ?
    uvec2( 0u, 0u ) :
    rigid_constraint_get( iter + constraint_index );
  const vec3 p0 = is_end ? vec3( 0, 0, 0 ) : particle_pool[ particle_offset + dc.x ].position;
  const vec3 center_of_mass0 = is_end ? vec3( 0, 0, 0 ) : rigid_pool[ rigid_id ].center_of_mass;
  const vec4 q0 = is_end ? vec4( 0, 0, 0, 0 ) : rigid_pool[ rigid_id ].angular_orientation;
  const vec3 r0 = p0 - center_of_mass0;
  const vec3 n0 = is_end ? vec3( 0, 0, 0 ) : particle_pool[ particle_offset + dc.x ].normal;
  const float inv_m0 = is_end ? 0.0 : particle_pool[ particle_offset + dc.x ].w;
  const mat3 inv_i0 = mat3( matrix_pool[ rigid_pool[ rigid_id ].inversed_momentum_inertia_tensor ] );

  const uint rigid1 = is_end ? 0xFFFFFFFF : particle_pool[ dc.y ].rigid;
  const vec3 p1 = is_end ? vec3( 0, 0, 0 ) : particle_pool[ dc.y ].position;
  const vec3 center_of_mass1 = ( rigid1 == 0xFFFFFFFF ) ? p0 : rigid_pool[ rigid1 ].center_of_mass;
  const vec3 r1 = ( rigid1 == 0xFFFFFFFF ) ? vec3( 0, 0, 0 ) : p0 - center_of_mass1;
  const vec3 n1 = is_end ? vec3( 0, 0, 0 ) : particle_pool[ dc.y ].normal;
  const float inv_m1 = is_end ? 0.0 : particle_pool[ dc.y ].w;
  const mat3 inv_i1 = mat3( matrix_pool[ rigid_pool[ rigid1 ].inversed_momentum_inertia_tensor ] );
  
  float lambda =
    ( is_end || lambda_offset == 0xFFFFFFFF ) ?
    0.0 :
    rigid_lambda_get( lambda_iter + constraint_index );

  const float w0 = inv_m0 + dot( cross( r0, n0 ), inv_i0 * cross( r0, n0 ) );
  
  const float w1 = inv_m1 + dot( cross( r1, n1 ), inv_i1 * cross( r1, n1 ) );

  const float d = distance( p0, p1 );
  const float c = d;

  const float alpha = 100.0;

  const float alpha_dt2 = alpha / ( dt * dt );
  const float dl = ( -c - alpha_dt2 * lambda ) / ( w0 + w1 + alpha_dt2 );

  const vec3 p = n0 * dl;
  const vec3 dx = is_end ? vec3( 0, 0, 0 ) : p * inv_m0;
  const vec4 dq = is_end ? vec4( 0, 0, 0, 0 ) : quaternion_quaternion_mult( 1.0/2.0 * vec4( inv_i0 * cross( r0, p ), 0.0 ), q0 );

  if( !( is_end || lambda_offset == 0xFFFFFFFF ) ) {
    rigid_lambda_set( lambda_iter + constraint_index, is_end ? 0.0 : lambda + dl );
  }

  vec3 dx_sum = subgroupAdd( dx );
  uint dx_count = subgroupAdd( dx == vec3( 0, 0, 0 ) ? 0u : 1u );
  vec4 dq_sum = subgroupAdd( dq );
  uint dq_count = subgroupAdd( dq == vec4( 0, 0, 0, 0 ) ? 0u : 1u );

  return rigid_collision_dx_dq(
    dx_sum,
    dq_sum,
    dx_count,
    dq_count
  );
}

// per particle
rigid_collision_dx_dq rigid_border_dx(
  uint rigid_id,
  uint particle_offset,
  uint unique_vertex_count,
  aabb_type aabb
) {
  const vec3 center_of_mass0 = rigid_pool[ rigid_id ].center_of_mass;
  const vec4 q0 = rigid_pool[ rigid_id ].angular_orientation;
  const mat3 inv_i0 = mat3( matrix_pool[ rigid_pool[ rigid_id ].inversed_momentum_inertia_tensor ] );
  const float alpha = 100.0;
  const float dt = 1.0/60.0;
  
  vec3 dx = vec3( 0.0, 0.0, 0.0 );
  vec4 dq = vec4( 0.0, 0.0, 0.0, 0.0 );
  uint dx_count;
  uint dq_count;
  for( uint vertex_id = gl_SubgroupInvocationID; vertex_id < unique_vertex_count; vertex_id += gl_SubgroupSize ) {
    const float inv_m0 = particle_pool[ particle_offset + vertex_id ].w;
    const vec3 p0 = particle_pool[ particle_offset + vertex_id ].position;
    const vec3 n0 = particle_pool[ particle_offset + vertex_id ].normal;
    const vec3 r0 = p0 - center_of_mass0;
    if( p0.x > aabb.max.x ) {
      const vec3 p = vec3( -( p0.x - aabb.max.x ), 0, 0 ); 
      const vec3 dx_ = p;
      const vec4 dq_ = quaternion_quaternion_mult( 1.0/2.0 * vec4( cross( r0, p ), 0.0 ), q0 );
      dx += dx_;
      dq += dq_;
      dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
      dq_count += ( dq_ == vec4( 0, 0, 0, 0 ) ) ? 0u : 1u;
    }
    else if( p0.x < aabb.min.x ) {
      const vec3 p = vec3( -( p0.x - aabb.min.x ), 0, 0 ); 
      const vec3 dx_ = p;
      const vec4 dq_ = quaternion_quaternion_mult( 1.0/2.0 * vec4( cross( r0, p ), 0.0 ), q0 );
      dx += dx_;
      dq += dq_;
      dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
      dq_count += ( dq_ == vec4( 0, 0, 0, 0 ) ) ? 0u : 1u;
    }
    if( p0.y > aabb.max.y ) {
      const vec3 p = vec3( 0, -( p0.y - aabb.max.y ), 0 ); 
      const vec3 dx_ = p;
      const vec4 dq_ = quaternion_quaternion_mult( 1.0/2.0 * vec4( cross( r0, p ), 0.0 ), q0 );
      dx += dx_;
      dq += dq_;
      dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
      dq_count += ( dq_ == vec4( 0, 0, 0, 0 ) ) ? 0u : 1u;
    }
    else if( p0.y < aabb.min.y ) {
      const vec3 p = vec3( 0, -( p0.y - aabb.min.y ), 0 ); 
      const vec3 dx_ = p;
      const vec4 dq_ = quaternion_quaternion_mult( 1.0/2.0 * vec4( cross( r0, p ), 0.0 ), q0 );
      dx += dx_;
      dq += dq_;
      dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
      dq_count += ( dq_ == vec4( 0, 0, 0, 0 ) ) ? 0u : 1u;
    }
    if( p0.z > aabb.max.z ) {
      const vec3 p = vec3( 0, 0, -( p0.z - aabb.max.z ) ); 
      const vec3 dx_ = p;
      const vec4 dq_ = quaternion_quaternion_mult( 1.0/2.0 * vec4( cross( r0, p ), 0.0 ), q0 );
      dx += dx_;
      dq += dq_;
      dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
      dq_count += ( dq_ == vec4( 0, 0, 0, 0 ) ) ? 0u : 1u;
    }
    else if( p0.z < aabb.min.z ) {
      const vec3 p = vec3( 0, 0, -( p0.z - aabb.min.z ) ); 
      const vec3 dx_ = p;
      const vec4 dq_ = quaternion_quaternion_mult( 1.0/2.0 * vec4( cross( r0, p ), 0.0 ), q0 );
      dx += dx_;
      dq += dq_;
      dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
      dq_count += ( dq_ == vec4( 0, 0, 0, 0 ) ) ? 0u : 1u;
    }
  }

  dx = subgroupAdd( dx );
  dq = subgroupAdd( dq );
  dx_count = subgroupAdd( dx_count );
  dq_count = subgroupAdd( dq_count );
  return rigid_collision_dx_dq(
    dx,
    dq,
    dx_count,
    dq_count
  );
}

#endif

