#ifndef GCT_SHADER_OMNISHADOW_H
#define GCT_SHADER_OMNISHADOW_H

#include "constants.h"
#include "poisson_disk.h"
#include "random.h"

layout(set=1, binding = 18) uniform samplerCube shadow;

mat2 get_rotate_matrix2( float angle ) {
  float s = sin( angle );
  float c = cos( angle );
  return mat2( c, s, -s, c );
}

mat3 get_rotate_matrix3( vec3 center ) {
  vec3 normal = normalize( center );
  vec3 guide;
  if( normal.y > 0.7071067811865475 ) {
    guide = vec3( 1.0, 0.0, 0.0 );
  }
  else {
    guide = vec3( 0.0, 1.0, 0.0 );
  }
  vec3 tangent = cross( guide, normal );
  tangent = normalize( tangent );
  vec3 binormal = cross( tangent, normal );
  return mat3( tangent, binormal, normal );
}

float pcss_omni( vec3 pos, vec3 light_pos, float light_size ) {
  vec3 p = pos - light_pos;
  mat2 rot2 = get_rotate_matrix2( rand1( p.xy ) * 2 * pi );
  mat3 rot3 = get_rotate_matrix3( p );
  float receiver_distance = length( p );
  float average_occluder_distance = 0;
  int occluder_count = 0;
  for( int i = 0; i < poisson_disk_sample_count; i++ ) {
    vec3 offset = rot3 * vec3( ( rot2 * poisson_disk[ i ] ) * light_size, 0.0 );
    float occluder_distance = texture( shadow, p + offset ).r;
    if( occluder_distance < receiver_distance ) {
      average_occluder_distance += occluder_distance;
      occluder_count++;
    }
  }
  if( occluder_count == 0 ) return 1.0;
  else average_occluder_distance /= occluder_count;
  float penumbra_width = ( receiver_distance - average_occluder_distance ) / receiver_distance;
  float filter_size = penumbra_width * light_size;
  float sum = 0;
  for( int i = 0; i < poisson_disk_sample_count; i++ ) {
    vec3 offset = rot3 * vec3( ( rot2 * poisson_disk[ i ] ) * filter_size, 0.0 );
    float occluder_distance = texture( shadow, p + offset ).r;
    sum += ( occluder_distance < receiver_distance ) ? 0 : 1;
  }
  return sum / poisson_disk_sample_count;
}

float pcf_omni( vec3 pos, vec3 light_pos, float light_size ) {
  vec3 p = pos - light_pos;
  mat2 rot2 = get_rotate_matrix2( rand1( p.xy ) * 2 * pi );
  mat3 rot3 = get_rotate_matrix3( p );
  float receiver_distance = length( p );
  float sum = 0;
  for( int i = 0; i < poisson_disk_sample_count; i++ ) {
    vec3 offset = rot3 * vec3( ( rot2 * poisson_disk[ i ] ) * light_size, 0.0 );
    float occluder_distance = texture( shadow, p + offset ).x;
    sum += ( occluder_distance < receiver_distance ) ? 0 : 1;
  }
  return sum / poisson_disk_sample_count;
}

float simple_shadow_omni( vec3 pos, vec3 light_pos ) {
  vec3 p = pos - light_pos;
  float distance = length( p );
  float projected = texture( shadow, p ).x;
  if( projected < distance ) {
    return 0.0;
  }
  else {
    return 1.0;
  }
}

#endif

