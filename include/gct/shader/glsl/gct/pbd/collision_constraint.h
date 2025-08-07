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
  const uint dc =
    is_end ?
    0u :
    constraint_get( iter + constraint_index );
  const vec3 p0 = particle_pool[ particle_offset + particle_id ].position;
  const vec3 pp0 = particle_pool[ particle_offset + particle_id ].previous_position;
  const vec3 p1 = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].position;


  vec3 n1 = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].normal;
  const bool has_normal = /*false;*/ n1 != vec3( 0.0, 0.0, 0.0 );

  n1 =
    has_normal ?
    n1 :
    normalize( p0 - p1 );
  const bool front = dot( ( pp0 - p1 ), n1 ) >= 0.0;
  n1 = front ? n1 : -n1;
  const float thickness = 0.1;
  const float w0 = particle_pool[ particle_offset + particle_id ].w;
  const float w1 = particle_pool[ ( is_end ? ( particle_offset + particle_id ) : dc ) ].w;
  const float d = distance( p0, p1 );

  const float dx_length =
    -( w0*( dot( n1, p0 - p1 ) - thickness ) )/( w0 + w1 );

  const vec3 dx =
    ( is_end ? vec3( 0, 0, 0 ) : ( dx_length > 0.0 ? n1 * dx_length : vec3( 0, 0, 0 ) ) );


  return subgroupAdd( vec4( dx, ( dx == vec3( 0, 0, 0 ) ) ? 0.0 : 1.0 ) );
}

// per particle
vec4 pbd_border_dx(
  uint particle_id,
  uint particle_offset,
  aabb_type aabb
) {
  vec3 dx = vec3( 0.0, 0.0, 0.0 );
  uint dx_count = 0u;
  const float inv_m0 = particle_pool[ particle_offset + particle_id ].w;
  const vec3 p0 = particle_pool[ particle_offset + particle_id ].position;
  if( p0.x > aabb.max.x ) {
    const vec3 dx_ = vec3( -( p0.x - aabb.max.x ), 0, 0 ); 
    dx += dx_;
    dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
  }
  else if( p0.x < aabb.min.x ) {
    const vec3 dx_ = vec3( -( p0.x - aabb.min.x ), 0, 0 ); 
    dx += dx_;
    dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
  }
  if( p0.y > aabb.max.y ) {
    const vec3 dx_ = vec3( 0, -( p0.y - aabb.max.y ), 0 ); 
    dx += dx_;
    dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
  }
  else if( p0.y < aabb.min.y ) {
    const vec3 dx_ = vec3( 0, -( p0.y - aabb.min.y ), 0 ); 
    dx += dx_;
    dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
  }
  if( p0.z > aabb.max.z ) {
    const vec3 dx_ = vec3( 0, 0, -( p0.z - aabb.max.z ) ); 
    dx += dx_;
    dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
  }
  else if( p0.z < aabb.min.z ) {
    const vec3 dx_ = vec3( 0, 0, -( p0.z - aabb.min.z ) );
    dx += dx_;
    dx_count += ( dx_ == vec3( 0, 0, 0 ) ) ? 0u : 1u;
  }
  return vec4( dx, float( dx_count ) );
}
#endif

