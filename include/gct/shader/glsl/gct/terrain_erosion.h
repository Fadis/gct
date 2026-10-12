#ifndef GCT_SHADER_TERRAIN_EROSION_H
#define GCT_SHADER_TERRAIN_EROSION_H

#ifdef __cplusplus
#include <gct/shader/glsl/gct/simplex_noise.h>
#include <gct/shader/glsl/gct/cellular_noise.h>
#include <gct/shader/glsl/gct/neighbor.h>
#include <glm/common.hpp>
#include <glm/geometric.hpp>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <gct/dummy_image.hpp>
using namespace glm;
using namespace gct;
#else
#include <gct/simplex_noise.h>
#include <gct/cellular_noise.h>
#include <gct/neighbor.h>
#define GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#include <gct/scene_graph/image_pool.h>
#endif


/*
 *  input : vec4( hij, 0, 0, 0 )
 *  output : vec4( hij, delta_h_per_di, 0, 0 )
 */
void thermal_erosion_calc_dmax(
  int src_image,
  int dest_image,
  ivec2 p,
  float T,
  float c
) {
  float center_h = imageLoad( image_pool_2d[ src_image ], p ).x;
  float dmax = 0.0f;
  float dtotal = 0.0f;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    float neighbor_h = imageLoad( image_pool_2d[ src_image ], p + neumann_neighbor[ i ] ).x;
    float di = center_h - neighbor_h;
    if( di <= T ) di = 0.0f;
    dmax = max( dmax, di );
    dtotal += di;
  }
  float delta_h_per_di = ( dtotal > 0.0f ) ? ( c * ( dmax - T ) / dtotal ) : 0.0f;

  imageStore( image_pool_2d[ dest_image ], p, vec4( center_h, delta_h_per_di, 0.0f, 0.0f ) );
}

/*
 *  input : vec4( hij, delta_h_per_di, 0, 0 )
 *  output : vec4( hij, 0, 0, 0 )
 */
void thermal_erosion_update_height(
  int src_image,
  int dest_image,
  ivec2 p,
  float T
) {
  const float c = 0.5f;
  const vec3 center = imageLoad( image_pool_2d[ src_image ], p );
  const float center_h = center.x;
  const float center_delta_h_per_di = center.y;
  float delta_h = 0.0f;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    const vec3 neighbor = vec3( imageLoad( image_pool_2d[ src_image ], p + neumann_neighbor[ i ] ) );
    const float neighbor_h = neighbor.x;
    const float neighbor_delta_h_per_di = neighbor.y;
    {
      float di = neighbor_h - center_h;
      if( di <= T ) di = 0.0f;
      delta_h += neighbor_delta_h_per_di * di;
      //if( di > 0.0f )
      //  std::cout << "debug : " << "(" << p.x << "," << p.y << ")" << " " << i << " incoming " << delta_h << std::endl;
    }
    {
      float di = center_h - neighbor_h;
      if( di <= T ) di = 0.0f;
      delta_h -= center_delta_h_per_di * di;
      //if( di > 0.0f )
      //  std::cout << "debug : " << "(" << p.x << "," << p.y << ")" << " " << i << " outgoing " << delta_h << std::endl;
    }
  }
  //std::cout << "total : " << "(" << p.x << "," << p.y << ")" << " " << " outgoing " << delta_h << std::endl;
  float new_h = center_h + delta_h;
  imageStore( image_pool_2d[ dest_image ], p, vec4( new_h, 0.0f, 0.0f, 0.0f ) );
}

/*
 *  input : vec4( hij, wij, mij, 0 )
 *  output : vec4( hij, wij, mij, a )
 */
void hydraulic_erosion_rain(
  int src_image,
  int dest_image,
  ivec2 p,
  float Kr,
  float Ks
) {
  const vec3 v = vec3( imageLoad( image_pool_2d[ src_image ], p ) );
  float h = v.x;
  float w = v.y;
  //float m = v.z;

  float new_w = w + Kr;
  float new_h = h - Ks * new_w;
  //float new_m = m + Ks * new_w;
  float a = new_h + new_w;
  //std::cout << Ks * w << std::endl;
  //if( p.x == 16 && p.y == 32 )
  //  std::cout << "debug 0 : h = " << h << " -> " << new_h << " w = " << w << " -> " << new_w << " a = " << a << std::endl;
  imageStore( image_pool_2d[ dest_image ], p, vec4( new_h, new_w, 0, a ) );
}

/*
 *  input : vec4( hij, wij, mij, a )
 *  output : vec4( dw/di, 0, 0, 0 )
 */
void hydraulic_erosion_flow(
  int src_image,
  int dest_image,
  ivec2 p
) {
  const vec4 v = imageLoad( image_pool_2d[ src_image ], p );
  float h = v.x;
  float w = v.y;
  float m = v.z;
  float a = h + w;

  float dtotal = 0.0f;
  float aaverage = a;
  int edge_count = 1u;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    float ai = imageLoad( image_pool_2d[ src_image ], p + neumann_neighbor[ i ] ).w;
    float di = a - ai;
    aaverage += ( di > 0.0f ) ?  ai : 0.0f;
    edge_count += ( di > 0.0f ) ? 1 : 0;
    dtotal += max( di, 0.0f );
  }
  aaverage /= edge_count;
  float delta_a = a - aaverage;

  float delta_w_per_di = ( dtotal > 0.0f ) ? ( min( w, delta_a ) / dtotal ) : 0.0f;
  if( std::isnan( delta_w_per_di ) || std::isinf( delta_w_per_di ) ) throw -1;
  imageStore( image_pool_2d[ dest_image ], p, vec4( delta_w_per_di, 0, 0, 0 ) );
}
void hydraulic_erosion_flow2(
  int src_image,
  int dest_image,
  ivec2 p
) {
  const vec4 center = imageLoad( image_pool_2d[ src_image ], p );
  float center_h = center.x;
  float center_w = center.y;
  float center_a = center.w;
  float dmax = 0.0f;
  uint lowest = 0u;
  float flow = 0.0f;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    const ivec2 neighbor_p = p + neumann_neighbor[ i ];
    float neighbor_h = imageLoad( image_pool_2d[ src_image ], neighbor_p ).x;
    float neighbor_a = imageLoad( image_pool_2d[ src_image ], neighbor_p ).w;
    float di = center_a - neighbor_a;
    if( di > dmax ) {
      dmax = di;
      lowest = i;
      flow = ( di > center_w ) ? center_w : ( center_a - ( center_a + neighbor_a ) * 0.5f );
     // std::cout << "debug -1 : " << p.x << " " << p.y << " " << neighbor_p.x << " " << neighbor_p.y << " " << center_a << " " << neighbor_a << " " << dmax << " " << di << " " << flow << " " << center_h << " " << neighbor_h << " " << center_w << " " << i << std::endl;
      if( center_w < flow ) std::abort();
      //if( p.x == 16 && p.y == 32 )
      //  std::cout << "debug 1 : h = " << center_h << " w = " << center_w << " nh = " << neighbor_h << " na = " << neighbor_a << " di = " << di << " lowest = " << lowest << " flow = " << flow << std::endl;
    }
    //else {
      //if( p.x == 16 && p.y == 32 )
      //  std::cout << "rej a = " << center_a << " na = " << neighbor_a << " i = " << i << " lowest = " <<  lowest << std::endl;
    //}
  }
  imageStore( image_pool_2d[ dest_image ], p, vec4( flow, float( lowest ), 0, 0 ) );
}

/*
 *  input : vec4( hij, wij, mij, a )
 *  input : vec4( dw/di, 0, 0, 0 )
 *  output : vec4( hij, wij, mij, 0 )
 */
void hydraulic_erosion_calc_update(
  int src_image,
  int dwdi_image,
  int dest_image,
  ivec2 p,
  float Ke,
  float Kc
) {
  const vec4 center_v = imageLoad( image_pool_2d[ src_image ], p );
  const float center_h = center_v.x;
  const float center_w = center_v.y;
  const float center_m = center_v.z;
  const float center_a = center_v.w;
  const float center_delta_w_per_di = imageLoad( image_pool_2d[ dwdi_image ], p ).x;

  float new_w = center_w;
  float new_m = center_m;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    const ivec2 neighbor_p = p + neumann_neighbor[ i ];
    const vec4 neighbor_v = imageLoad( image_pool_2d[ src_image ], neighbor_p );
    const float neighbor_h = neighbor_v.x;
    const float neighbor_w = neighbor_v.y;
    const float neighbor_m = neighbor_v.z;
    const float neighbor_a = neighbor_v.w;
    const float neighbor_delta_w_per_di = imageLoad( image_pool_2d[ dwdi_image ], p + neumann_neighbor[ i ] ).x;
    {
      float di = max( neighbor_a - center_a, 0.0f ); // neighbor is higher 
      float delta_wi = neighbor_delta_w_per_di * di;
      float delta_mi = neighbor_m * delta_wi / neighbor_w;
      new_w += delta_wi;
      new_m += delta_mi;
    }
    {
      float di = max( center_a - neighbor_a, 0.0f ); // neighbor is lower
      float delta_wi = center_delta_w_per_di * di;
      float delta_mi = center_m * delta_wi / center_w;
      new_w -= delta_wi;
      new_m -= delta_mi;
    }
  }

  /*if( center_w != new_w ) {
    std::cout << p.x << "," << p.y << " " << center_m << " " << new_m << std::endl;
  }*/

  new_w *= ( 1.0f - Ke );
  const float mmax = Kc * new_w;
  const float delta_m = max( 0.0f, new_m - mmax );
  new_m -= delta_m;
  /*if( delta_m > 0.0f ) {
    std::cout << p.x << "," << p.y << " " << delta_m << std::endl;
  }*/
  float new_h = center_h + delta_m;
  imageStore( image_pool_2d[ dest_image ], p, vec4( new_h, new_w, new_m, 0 ) );
}

void hydraulic_erosion_calc_update2(
  int src_image,
  int dwdi_image,
  int dest_image,
  ivec2 p,
  float Ke,
  float Kc
) {
  const vec4 center_v = imageLoad( image_pool_2d[ src_image ], p );
  const float center_h = center_v.x;
  const float center_w = center_v.y;
  //const float center_m = center_v.z;
  const float center_a = center_v.w;
  const vec4 center_v2 = imageLoad( image_pool_2d[ dwdi_image ], p );
  const float center_flow = center_v2.x;
  const int center_lowest = int( center_v2.y );

  float delta_w = 0.0f;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    const ivec2 neighbor_p = p + neumann_neighbor[ i ];
    const vec4 neighbor_v = imageLoad( image_pool_2d[ src_image ], neighbor_p );
    const float neighbor_h = neighbor_v.x;
    const vec4 neighbor_v2 = imageLoad( image_pool_2d[ dwdi_image ], neighbor_p );
    const float neighbor_flow = neighbor_v2.x;
    const int neighbor_lowest = int( neighbor_v2.y );
    //std::cout << "debug 0c : " << p.x << " " << p.y << " " << neighbor_p.x << " " << neighbor_p.y << " " << neighbor_h << " " << neighbor_flow << " " << neighbor_lowest << std::endl;
    {
      const ivec2 lowest = neighbor_p + neumann_neighbor[ int( neighbor_lowest ) ];
      float di = neighbor_h - center_h;
      if( lowest == p ) {
        delta_w += neighbor_flow;
        //if( p.x == 16 && p.y == 32 )
        //  std::cout << "debug 3 : h = " << center_h << " w = " << center_w << " nh = " << neighbor_h << " nf = " << neighbor_flow << " delta_w = " << delta_w << std::endl;
        //std::cout << "debug 0a : " << p.x << " " << p.y << " " << neighbor_p.x << " " << neighbor_p.y << " " << neighbor_flow << " " << delta_w << std::endl;
      }
    }
    {
      const ivec2 lowest = p + neumann_neighbor[ int( center_lowest ) ];
      float di = center_h - neighbor_h;
      if( lowest == neighbor_p ) {
        delta_w -= center_flow;
        //if( p.x == 16 && p.y == 32 )
        //  std::cout << "debug 4 : h = " << center_h << " w = " << center_w << " nh = " << neighbor_h << " f = " << center_flow << " delta_w = " << delta_w << std::endl;
        //std::cout << "debug 0b : " << p.x << " " << p.y << " " << neighbor_p.x << " " << neighbor_p.y << " " << neighbor_flow << " " << delta_w << std::endl;
      }
    }
  }

  float new_w = center_w + delta_w;
  //std::cout << "debug 1 : " << new_w << " " << center_w << " " << delta_w << std::endl;
  const float delta_m = Kc * new_w;
  new_w *= ( 1.0f - Ke );
  //std::cout << "debug 2 : " << new_w << " " << center_w << " " << delta_w << std::endl;

  if( new_w < 0.0f ) std::abort();

  //const float mmax = Kc * new_w;
  //const float delta_m = max( 0.0f, center_m - mmax );
  //float new_m = center_m - delta_m;
 
  //if( new_m < 0.0f ) std::abort();
  float new_h = center_h + delta_m;
 
  //if( p.x == 16 && p.y == 32 )
  //  std::cout << "debug 5 : h = " << center_h << " -> " << new_h << " w = " << center_w << " -> " << new_w << " delta_w = " << delta_w << " delta_m = " << delta_m << " " <<  std::endl;

  imageStore( image_pool_2d[ dest_image ], p, vec4( new_h, new_w, 0, 0 ) );
}


/*
 *  input : vec4( hij, 0, 0, 0 )
 *  output : vec4( hij, delta_h_per_di, 0, 0 )
 */
void simple_hydraulic_erosion_calc_dmax(
  int src_image,
  int dest_image,
  ivec2 p,
  float T
) {
  float center_h = imageLoad( image_pool_2d[ src_image ], p ).x;
  float dmax = 0.0f;
  uint lowest = 0u;
  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    float neighbor_h = imageLoad( image_pool_2d[ src_image ], p + neumann_neighbor[ i ] ).x;
    float di = center_h - neighbor_h;
    if( di > dmax ) {
      dmax = di;
      lowest = i;
    }
    //if( di < 0.0f || di > T ) di = 0.0f;
    //dmax = max( dmax, di );
  }
  float delta_h = 0.5 * dmax;

  imageStore( image_pool_2d[ dest_image ], p, vec4( center_h, delta_h, float( lowest ), 0.0f ) );
}

/*
 *  input : vec4( hij, delta_h, 0, 0 )
 *  output : vec4( hij, 0, 0, 0 )
 */
void simple_hydraulic_erosion_update(
  int src_image,
  int dest_image,
  ivec2 p,
  float T
) {
  const vec3 center = imageLoad( image_pool_2d[ src_image ], p );
  const float center_h = center.x;
  const float center_delta_h = center.y;
  float delta_h = 0.0f;

  for( uint i = 0u; i != neumann_neighbor_count; ++i ) {
    const ivec2 neighbor_p = p + neumann_neighbor[ i ];
    const vec3 neighbor = vec3( imageLoad( image_pool_2d[ src_image ], neighbor_p ) );
    const float neighbor_h = neighbor.x;
    const float neighbor_delta_h = neighbor.y;
    {
      const ivec2 lowest = neighbor_p + neumann_neighbor[ int( neighbor.z ) ];
      float di = neighbor_h - center_h;
      if( lowest == p && di > 0.0f && di <= T ) {
        delta_h += neighbor_delta_h;
      }
    }
    {
      const ivec2 lowest = p + neumann_neighbor[ int( center.z ) ];
      float di = center_h - neighbor_h;
      if( lowest == neighbor_p && di > 0.0f && di <= T ) {
        delta_h -= center_delta_h;
      }
    }
  }
  /*if( delta_h != 0.0f ) {
    std::cout << p.x << " " << p.y << " " << delta_h << std::endl;
  }*/
  float new_h = center_h + delta_h;
  imageStore( image_pool_2d[ dest_image ], p, vec4( new_h, 0.0f, 0.0f, 0.0f ) );
}


#endif

