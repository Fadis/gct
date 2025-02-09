#ifndef GCT_SHADER_HBAO
#define GCT_SHADER_HBAO

#include <gct/constants.h>

float hbao( uint center ) {
  const float center_d = distance( hbao_cache[ center + 1 ].xy, hbao_cache[ center - 1 ].xy );
  const float t = ( center_d > 0.0 ) ? atan( ( -hbao_cache[ center + 1 ].z - -hbao_cache[ center - 1 ].z)/center_d ) : 0.0;
  float l = t;
  for( uint i = 0; i != hbao_kernel_size; i++ ) {
    const float z_distance = -hbao_cache[ center - i - 1 ].z - -hbao_cache[ center ].z;
    const float distance_mask = 1.0 / max( z_distance * z_distance, 1.0 );
    const float d = distance( hbao_cache[ center - i - 1 ].xy, hbao_cache[ center ].xy );
    const float h = ( d > 0.0 ) ? atan( ( -hbao_cache[ center - i - 1 ].z - -hbao_cache[ center ].z)/d ) : -pi/2.0;
    l = max( l, mix( -h, t, mix( 1.0, float( i )/float( hbao_kernel_size ), distance_mask ) ) );
  }
  float r = -t;
  for( uint i = 0; i != hbao_kernel_size; i++ ) {
    const float z_distance = -hbao_cache[ center + i + 1 ].z - -hbao_cache[ center ].z;
    const float distance_mask = 1.0 / max( z_distance * z_distance, 1.0 );
    const float d = distance( hbao_cache[ center + i + 1 ].xy, hbao_cache[ center ].xy );
    const float h = ( d > 0.0 ) ? atan( ( -hbao_cache[ center + i + 1 ].z - -hbao_cache[ center ].z)/d ) : -pi/2.0;
    r = max( r, mix( -h, -t, mix( 1.0, float( i )/float( hbao_kernel_size ), distance_mask ) ) );
  }
  return ( -cos( ( pi - l + t ) ) -  -cos( r + t ) )/2;
}

#endif

