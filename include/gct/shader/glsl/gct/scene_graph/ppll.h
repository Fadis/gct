#ifndef GCT_SHADER_SCENE_GRAPH_PPLL_H
#define GCT_SHADER_SCENE_GRAPH_PPLL_H

#include <gct/primitive_value.h>
#include <gct/gbuffer_value.h>
#include <gct/scene_graph/image_pool.h>
#include <gct/scene_graph/ppll_state.h>
#include <gct/scene_graph/ppll_state_pool.h>
#include <gct/depth.h>
#include <gct/pre_dof_pixel.h>
#include <gct/gbuffer_format.h>

const uint ppll_array_layer_count = 5;

#ifndef GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#error "GCT_USE_IMAGE_POOL_WITHOUT_FORMAT is required"
#endif

struct ppll_image {
  uint gbuffer;
  uint position;
  uint start;
  uint next;
};

struct ppll_iter {
  uint state_id;
  ppll_image image;
  ivec2 image_pos;
  uint index;
  uint active_layer;
};

ppll_iter ppll_begin(
  uint state_id,
  ppll_image image,
  ivec2 image_pos,
  uint active_layer
) {
  const uint start = imageLoad( image_pool_2du[ nonuniformEXT( image.start ) ], image_pos ).r;
  return ppll_iter(
    state_id,
    image,
    image_pos,
    start,
    active_layer
  );
}

ppll_iter ppll_begin(
  uint state_id,
  ppll_image image,
  ivec2 image_pos
) {
  return ppll_begin(
    state_id,
    image,
    image_pos,
    GCT_GBUFFER_ALBEDO_ALPHA |
    GCT_GBUFFER_NORMAL |
    GCT_GBUFFER_EMISSIVE_OCCLUSION |
    GCT_GBUFFER_METALLIC_ROUGHNESS_ID |
    GCT_GBUFFER_OPTFLOW_MARK
  );
}

ppll_iter ppll_direct(
  uint state_id,
  ppll_image image,
  ivec2 image_pos,
  uint active_layer
) {
  const ivec2 gbuffer_size = imageSize( image_pool_2du[ nonuniformEXT( image.gbuffer ) ] ).xy;
  const uint index = image_pos.x + image_pos.y * gbuffer_size.x;
  bool used = ppll_state_pool[ state_id ].current_tail >= index;
  return ppll_iter(
    state_id,
    image,
    ivec2( -1, -1 ),
    used ? index : 0u,
    active_layer
  );
}

ppll_iter ppll_direct(
  uint state_id,
  ppll_image image,
  ivec2 image_pos
) {
  return ppll_direct(
    state_id,
    image,
    image_pos,
    GCT_GBUFFER_ALBEDO_ALPHA |
    GCT_GBUFFER_NORMAL |
    GCT_GBUFFER_EMISSIVE_OCCLUSION |
    GCT_GBUFFER_METALLIC_ROUGHNESS_ID |
    GCT_GBUFFER_OPTFLOW_MARK
  );
}

ppll_iter ppll_next(
  ppll_iter iter
) {
  const ivec2 gbuffer_size = imageSize( image_pool_2du[ nonuniformEXT( iter.image.gbuffer ) ] ).xy;
  const uint next =
    /*( iter.index == 0u ) ?
    imageLoad(
      image_pool_2du[ nonuniformEXT( iter.image.start ) ],
      iter.image_pos
    ).x :*/
    imageLoad(
      image_pool_2du[ nonuniformEXT( iter.image.next ) ],
      ivec2( iter.index % gbuffer_size.x, iter.index / gbuffer_size.x )
    ).x;
  return ppll_iter(
    iter.state_id,
    iter.image,
    iter.image_pos,
    next,
    iter.active_layer
  );
}

bool ppll_is_end(
  ppll_iter iter
) {
  return iter.index == 0u;
}

bool ppll_is_nearest(
  ppll_iter iter
) {
  return ppll_is_end( ppll_next( iter ) );
}

ppll_iter ppll_nearest(
  ppll_iter iter
) {
  const ivec2 gbuffer_size = imageSize( image_pool_2du[ nonuniformEXT( iter.image.gbuffer ) ] ).xy;
  uint prev = 0u;
  uint next = iter.index;
  for( uint i = 0u; i != 32u; ++i ) {
    if( next != 0u ) {
      prev = next;
      next = imageLoad(
        image_pool_2du[ nonuniformEXT( iter.image.next ) ],
        ivec2( next % gbuffer_size.x, next / gbuffer_size.x )
      ).x;
    }
  }
  iter.index = prev;
  return iter;
}

ppll_iter ppll_nearest_in_slice(
  ppll_iter iter,
  float depth_threshold
) {
  const ivec2 gbuffer_size = imageSize( image_pool_2du[ nonuniformEXT( iter.image.gbuffer ) ] ).xy;
  uint prev = 0u;
  uint next = iter.index;
  for( uint i = 0u; i != 32u; ++i ) {
    if( next != 0u ) {
      float existing_depth = imageLoad(
        image_pool_2d[ nonuniformEXT( iter.image.position ) ],
        ivec2( next % gbuffer_size.x, next / gbuffer_size.x )
      ).w;
      if( existing_depth > depth_threshold ) {
        prev = next;
        next = imageLoad(
          image_pool_2du[ nonuniformEXT( iter.image.next ) ],
          ivec2( next % gbuffer_size.x, next / gbuffer_size.x )
        ).x;
      }
    }
  }
  iter.index = prev;
  return iter;
}

bool ppll_equal(
  ppll_iter l,
  ppll_iter r
) {
  return l.index == r.index;
}

ivec2 ppll_get_pixel_pos(
  ppll_iter iter
) {
  const ivec2 gbuffer_size = imageSize( image_pool_2du[ nonuniformEXT( iter.image.gbuffer ) ] ).xy;
  return ivec2( iter.index % gbuffer_size.x, iter.index / gbuffer_size.x );
}

void ppll_set(
  ppll_iter iter,
  uint image,
  vec4 value
) {
  imageStore( image_pool_2d[ nonuniformEXT( image ) ], ppll_get_pixel_pos( iter ), value );
}

vec4 ppll_get(
  ppll_iter iter,
  uint image
) {
  return imageLoad( image_pool_2d[ nonuniformEXT( image ) ], ppll_get_pixel_pos( iter ) );
}

void ppll_insert(
  ppll_iter iter,
  primitive_value p,
  float depth,
  vec2 input_id
) {
  const ivec2 gbuffer_size = imageSize( image_pool_2du[ nonuniformEXT( iter.image.gbuffer ) ] ).xy;
  const uint start = imageLoad( image_pool_2du[ nonuniformEXT( iter.image.start ) ], iter.image_pos ).r;
  uint prev = 0u;
  uint next = start;
  for( uint i = 0u; i != 32u; ++i ) {
    if( next != 0u ) {
      float existing_depth = imageLoad(
        image_pool_2d[ nonuniformEXT( iter.image.position ) ],
        ivec2( next % gbuffer_size.x, next / gbuffer_size.x )
      ).w;
      if( existing_depth > depth ) {
        prev = next;
        next = imageLoad(
          image_pool_2du[ nonuniformEXT( iter.image.next ) ],
          ivec2( next % gbuffer_size.x, next / gbuffer_size.x )
        ).x;
      }
    }
  }
  iter.index = atomicAdd( ppll_state_pool[ iter.state_id ].current_tail, 1u ) + 1u;
  const ivec2 pixel_pos = ppll_get_pixel_pos( iter );
  if( prev == 0u ) {
    imageStore(
      image_pool_2du[ nonuniformEXT( iter.image.start ) ],
      iter.image_pos,
      uvec4( iter.index, 0u, 0u, 0u )
    );
  }
  else {
    imageStore(
      image_pool_2du[ nonuniformEXT( iter.image.next ) ],
      ivec2( prev % gbuffer_size.x, prev / gbuffer_size.x ),
      uvec4( iter.index, 0u, 0u, 0u )
    );
  }
  //if( next != 0u ) {
    imageStore(
      image_pool_2du[ nonuniformEXT( iter.image.next ) ],
      pixel_pos,
      uvec4( next, 0u, 0u, 0u )
    );
  //}
  imageStore( image_pool_2d[ nonuniformEXT( iter.image.position ) ], pixel_pos, vec4( p.pos, depth ) );
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_POSITION_DEPTH ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_POSITION_DEPTH ) ),
      vec4( p.pos, depth )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_ALBEDO_ALPHA ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_ALBEDO_ALPHA ) ),
      p.albedo
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_NORMAL ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_NORMAL ) ),
      vec4( p.normal, 1.0 )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_EMISSIVE_OCCLUSION ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_EMISSIVE_OCCLUSION ) ),
      vec4( p.emissive, p.occlusion )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_METALLIC_ROUGHNESS_ID ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_METALLIC_ROUGHNESS_ID ) ),
      vec4( p.metallic, p.roughness, input_id.x, input_id.y )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_OPTFLOW_MARK ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_OPTFLOW_MARK ) ),
      vec4( p.optflow, 0.0 )
    );
  }

  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TANGENT ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TANGENT ) ),
      vec4( p.tangent, 0.0 )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD0_TEXCOORD1 ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD0_TEXCOORD1 ) ),
      vec4( p.texcoord[ 0 ], p.texcoord[ 1 ] )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD2_TEXCOORD3 ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD2_TEXCOORD3 ) ),
      vec4( p.texcoord[ 2 ], p.texcoord[ 3 ] )
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_COLOR0 ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_COLOR0 ) ),
      p.color[ 0 ]
    );
  }
  if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_COLOR1 ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_COLOR1 ) ),
      p.color[ 1 ]
    );
  }
}

vec4 ppll_get_component(
  ppll_iter iter,
  uint layer,
  vec4 default_value
) {
  const ivec2 pixel_pos = ppll_get_pixel_pos( iter );
  return
    gbuffer_has_layer( iter.active_layer, layer ) ?
    imageLoad(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3( pixel_pos, gbuffer_get_layer( iter.active_layer, layer ) )
    ):
    default_value;
}

float ppll_get_depth(
  ppll_iter iter
) {
  const ivec2 pixel_pos = ppll_get_pixel_pos( iter );
  return imageLoad( image_pool_2d[ nonuniformEXT( iter.image.position ) ], pixel_pos ).w;
}

vec4 ppll_get_pos_depth(
  ppll_iter iter
) {
  const ivec2 pixel_pos = ppll_get_pixel_pos( iter );
  return imageLoad( image_pool_2d[ nonuniformEXT( iter.image.position ) ], pixel_pos );
}

vec4 ppll_get_albedo(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_ALBEDO_ALPHA, vec4( 0, 0, 0, 0 ) );
}

vec3 ppll_get_normal(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_NORMAL, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 ppll_get_eo(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_EMISSIVE_OCCLUSION, vec4( 0, 0, 0, 0 ) );
}

vec4 ppll_get_mrid(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_METALLIC_ROUGHNESS_ID, vec4( 0, 0, 0, 0 ) );
}

vec3 ppll_get_optflow(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_OPTFLOW_MARK, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec3 ppll_get_tangent(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_TANGENT, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 ppll_get_texcoord01(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_TEXCOORD0_TEXCOORD1, vec4( 0, 0, 0, 0 ) );
}

vec4 ppll_get_texcoord23(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_TEXCOORD2_TEXCOORD3, vec4( 0, 0, 0, 0 ) );
}

vec4 ppll_get_color0(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_COLOR0, vec4( 0, 0, 0, 0 ) );
}

vec4 ppll_get_color1(
  ppll_iter iter
) {
  return ppll_get_component( iter, GCT_GBUFFER_COLOR1, vec4( 0, 0, 0, 0 ) );
}

gbuffer_value ppll_get(
  ppll_iter iter
) {
  const vec4 position_depth = imageLoad( image_pool_2d[ nonuniformEXT( iter.image.position ) ], ppll_get_pixel_pos( iter ) );
  const vec4 albedo = ppll_get_albedo( iter );
  const vec3 normal = ppll_get_normal( iter );
  const vec4 eo = ppll_get_eo( iter );
  const vec4 mrid = ppll_get_mrid( iter );
  const vec3 optflow = ppll_get_optflow( iter );
  const vec3 tangent = ppll_get_tangent( iter );
  const vec4 texcoord01 = ppll_get_texcoord01( iter );
  const vec4 texcoord23 = ppll_get_texcoord23( iter );
  const vec4 color0 = ppll_get_color0( iter );
  const vec4 color1 = ppll_get_color1( iter );
  gbuffer_value p;
  p.pos = position_depth.xyz;
  p.depth = position_depth.w;
  p.normal = normal;
  p.metallic = mrid.x;
  p.roughness = mrid.y;
  p.input_id = ivec2( mrid.zw );
  p.albedo = albedo;
  p.emissive = eo.xyz;
  p.occlusion = eo.w;
  p.optflow = optflow;
  p.tangent = tangent;
  p.texcoord[ 0 ] = texcoord01.xy;
  p.texcoord[ 1 ] = texcoord01.zw;
  p.texcoord[ 2 ] = texcoord23.xy;
  p.texcoord[ 3 ] = texcoord23.zw;
  p.color[ 0 ] = color0;
  p.color[ 1 ] = color1;
  return p;
}

pre_dof_pixel ppll_mix(
  ppll_iter iter,
  uint lighting_image,
  uint scattering_image,
  float focus,
  float znear,
  float zfar,
  float visible_range,
  vec3 ambient_factor,
  float ao
) {
  vec4 near_total = vec4( 0.0, 0.0, 0.0, 0.0 );
  vec4 far_total = vec4( 0.0, 0.0, 0.0, 0.0 );
  float near_depth = focus;
  float far_depth = zfar;
  for( uint i = 0u; i != 32u; i++ ) {
    const vec4 albedo = ppll_get_albedo( iter );
    const bool has_layer = !ppll_is_end( iter );
    const bool is_nearest = ppll_is_nearest( iter );
    const float depth = has_layer ? 
      decode_depth( ppll_get_depth( iter ), znear, zfar ) :
      zfar;
    const vec3 ambient =
      ( has_layer ) ?
      ambient_factor *
      ( is_nearest ? ao : 1.0 ) *
      albedo.xyz :
      vec3( 0.0, 0.0, 0.0 );
    const vec3 lighting = 
      ( has_layer ) ?
      ppll_get( iter, lighting_image ).xyz :
      vec3( 0.0, 0.0, 0.0 );
    const vec4 scat = 
      ( has_layer ) ?
      ppll_get( iter, scattering_image ) :
      vec4( 0.0, 0.0, 0.0, 0.0 );
    const vec4 radiance = 
      vec4( ( ambient + ( lighting.rgb ) ) * scat.w + scat.rgb, albedo.a );
    near_depth = min( depth, near_depth );
    far_depth = min( depth, far_depth );
    if( has_layer ) { 
      near_total.xyz = mix( near_total.xyz, radiance.xyz, albedo.a );
      if( depth < focus ) {
        near_total.a = ( 1.0 - ( 1.0 - near_total.a ) * ( 1.0 - albedo.a ) );
      }
      far_total.xyz = mix( far_total.xyz, radiance.xyz, albedo.a );
      far_total.a = ( 1.0 - ( 1.0 - far_total.a ) * ( 1.0 - albedo.a ) );
      iter = ppll_next( iter );
    }
  }
  near_depth = min( focus, near_depth + visible_range );
  far_depth = max( focus, far_depth - visible_range );
  return pre_dof_pixel(
    near_total,
    far_total,
    near_depth,
    far_depth
  );
}

#endif

