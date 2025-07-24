#ifndef GCT_SHADER_PBD_RIGID_CONSTRAINT_H
#define GCT_SHADER_PBD_RIGID_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/rigid_constraint.h>

struct rigid_collision_dx_dq {
  vec3 dx;
  vec4 dq;
  uint dx_count;
  uint dq_count;
};

rigid_collision_dx_dq rigid_collision_constraint_dx(
  uint rigid_id,
  uint particle_offset,
  uint constraint_offset,
  uint lambda_offset,
  float dt
) {
  const uint iter = rigid_constraint_begin( constraint_offset );
  const uint lambda_iter = rigid_constraint_begin( lambda_offset );
  const bool is_end = rigid_constraint_is_end( iter + gl_SubgroupInvocationID );
  const uvec2 dc =
    is_end ?
    uvec2( 0u, 0u ) :
    rigid_constraint_get( iter + gl_SubgroupInvocationID );
  const vec3 p0 = is_end ? vec3( 0, 0, 0 ) : particle_pool[ particle_offset + dc.x ].position;
  const vec3 center_of_mass0 = is_end ? vec3( 0, 0, 0 ) : rigid_pool[ rigid_id ].center_of_mass;
  const vec4 q0 = is_end ? vec3( 0, 0, 0 ) : rigid_pool[ rigid_id ].angular_orientation;
  const vec3 r0 = p0 - center_of_mass0;
  const vec3 n0 = is_end ? vec3( 0, 0, 0 ) : particle_pool[ particle_offset + dc.x ].normal;
  const float inv_m0 = is_end ? 0.0 : particle_pool[ particle_offset + dc.x ].w;
  const mat3 inv_i0 = mat3( 0.0 );

  const uitn rigid1 = is_end ? 0xFFFFFFFF : particle_pool[ dc.y ].rigid;
  const vec3 p1 = is_end ? vec3( 0, 0, 0 ) ? particle_pool[ dc.y ].position;
  const vec3 center_of_mass1 = ( rigid1 == 0xFFFFFFFF ) ? p0 : rigid_pool[ rigid1 ].center_of_mass;
  const vec3 r1 = ( rigid1 == 0xFFFFFFFF ) ? vec3( 0, 0, 0 ) ? p0 - center_of_mass1;
  const vec3 n1 = is_end ? vec3( 0, 0, 0 ) : partcle_pool[ dc.y ].normal;
  const float inv_m0 = is_end ? 0.0 : particle_pool[ dc.y ].w;
  const mat3 inv_i1 = mat3( 0.0 );
  
  float lambda = rigid_constraint_get_lambda( lambda_iter + gl_SubgroupInvocationID );

  const float w0 = inv_m0 + dot( cross( r0, n0 ), inv_i0 * cross( r0, n0 ) );
  
  const float w1 = inv_m1 + dot( cross( r1, n1 ), inv_i1 * cross( r1, n1 ) );

  const float d = distance( p0, p1 );
  const float c = d;

  const float alpha = ///// ???
  const float alpha_dt2 = alpha / ( dt * dt );
  const float dl = ( -c - alpha_dt2 * lambda ) / ( w0 + w1 + alpha_dt2 );

  const vec3 p = n0 * dl;
  const vec3 dx = is_end ? vec3( 0, 0, 0 ) : p * inv_m0p
  const vec4 dq = is_end ? vec4( 0, 0, 0, 0 ) : quaternion_quaternion_mult( 1.0/2.0 * vec4( inv_i0 * cross( r0, p ), 0.0 ), q0 );

  rigid_constraint_set_lambda( lambda_iter + gl_SubgroupInvocationID, is_end ? 0.0 : lambda + dl );

  vec3 dx_sum = subgroupAdd( dx );
  uint dx_count = subgroupAdd( dx == vec3( 0, 0, 0 ) ? 0u : 1u );
  vec4 dq_sum = subgroupAdd( dq );
  uint dx_count = subgroupAdd( dq == vec4( 0, 0, 0, 0 ) ? 0u : 1u );

  return rigid_collision_dx_dq(
    dx_sum,
    dq_sum,
    dx_count,
    dq_count
  );
}

#endif

