#ifndef GCT_SHADER_PBD_FLUID_CONSTRAINT_H
#define GCT_SHADER_PBD_FLUID_CONSTRAINT_H

#define GCT_ENABLE_8BIT_16BIT_STORAGE
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph.h>
#include <gct/scene_graph/accessor.h>
#include <gct/constraint.h>
#include <gct/poly6_kernel.h>

float pbd_fluid_get_rho(
  uint particle_id,
  uint particle_offset,
  uint constraint_offset,
  float radius
) {
  const uint iter = constraint_begin( particle_id, constraint_offset );
  const vec3 center = particle_pool[ particle_offset + particle_id ].position;
  float rho = 0.0;
  for( uint constraint_block_id = 0u; constraint_block_id != 4u; constraint_block_id++ ) {
    uint gcid = iter + gl_SubgroupSize * constraint_block_id + gl_SubgroupInvocationID;
    const bool is_end = constraint_is_end( gcid );
    const uint dc =
      is_end ?
      0u :
      constraint_get( gcid );
    const vec3 neighbor = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].position;
    rho += subgroupAdd( is_end ? 0.0 : poly6_kernel( center - neighbor, radius ) );
  }
  return rho;
}

float pbd_fluid_get_lambda(
  float rho0,
  uint particle_id,
  uint particle_offset,
  uint constraint_offset,
  float radius
) {
  const uint iter = constraint_begin( particle_id, constraint_offset );
  const vec3 center = particle_pool[ particle_offset + particle_id ].position;
  float dpici = 0.0;
  float dpjci = 0.0;
  {
    vec3 sum = vec3( 0.0, 0.0, 0.0 );
    for( uint constraint_block_id = 0u; constraint_block_id != 4u; constraint_block_id++ ) {
      uint gcid = iter + gl_SubgroupSize * constraint_block_id + gl_SubgroupInvocationID;
      const bool is_end = constraint_is_end( gcid );
      const uint dc =
        is_end ?
        0u :
        constraint_get( gcid );
      const vec3 neighbor = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].position;
      vec3 grad = is_end ? vec3( 0.0, 0.0, 0.0 ) : poly6_kernel_gradient( center - neighbor, radius ) / rho0;
      sum += subgroupAdd( grad );
      dpjci += subgroupAdd( dot( grad, grad ) );
    }
    dpici = dot( sum, sum );
  }
  const float eps = 1000.0;
  return
    -( pbd_fluid_get_rho( particle_id, particle_offset, constraint_offset, radius )/rho0 - 1.0f ) /
    ( dpici + dpjci + eps );
}

vec4 pbd_fluid_constraint(
  float rho0,
  uint particle_id,
  uint particle_offset,
  uint constraint_offset,
  float radius
) {
  const uint iter = constraint_begin( particle_id, constraint_offset );
  const vec3 center = particle_pool[ particle_offset + particle_id ].position;
  float lambda_i = pbd_fluid_get_lambda( rho0, particle_id, particle_offset, constraint_offset, radius );
  vec3 sum = vec3( 0.0f, 0.0f, 0.0f );
  uint count = 0;
  for( uint constraint_block_id = 0u; constraint_block_id != 4u; constraint_block_id++ ) {
    uint gcid = iter + gl_SubgroupSize * constraint_block_id + gl_SubgroupInvocationID;
    const bool is_end = constraint_is_end( gcid );
    const uint dc =
      is_end ?
      0u :
      constraint_get( gcid );
    const vec3 neighbor = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].position;
    float lambda_j =
      is_end ?
      0.0 :
      pbd_fluid_get_lambda( rho0, dc - particle_offset, particle_offset, constraint_offset, radius );
    vec3 dxdw = poly6_kernel_gradient( center - neighbor, radius );
    dxdw *= lambda_i + lambda_j;
    sum += subgroupAdd( is_end ? vec3( 0.0, 0.0, 0.0 ) : dxdw );
    count += subgroupAdd( is_end ? 0 : 1 );
  }
  sum /= rho0;
  return vec4( sum, float( count ) );
}

vec3 pbd_fluid_xsph_viscosity(
  uint particle_id,
  uint particle_offset,
  uint constraint_offset,
  float radius
) {
  const uint iter = constraint_begin( particle_id, constraint_offset );
  const vec3 center = particle_pool[ particle_offset + particle_id ].position;
  const vec3 center_v = particle_pool[ particle_offset + particle_id ].velocity;
  vec3 sum = vec3( 0.0f, 0.0f, 0.0f );
  uint count = 0;
  for( uint constraint_block_id = 0u; constraint_block_id != 4u; constraint_block_id++ ) {
    uint gcid = iter + gl_SubgroupSize * constraint_block_id + gl_SubgroupInvocationID;
    const bool is_end = constraint_is_end( gcid );
    const uint dc =
      is_end ?
      0u :
      constraint_get( gcid );
    const vec3 neighbor = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].position;
    const vec3 neighbor_v = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].velocity;
    const float w = poly6_kernel( center - neighbor, radius );
    const vec3 vij = neighbor_v - center_v;
    sum += subgroupAdd( is_end ? vec3( 0.0, 0.0, 0.0 ) : vij * w );
    count += subgroupAdd( is_end ? 0 : 1 );
  }
  sum *= 0.001;
  return sum;
}

#endif

