#ifndef GCT_SHADER_SCENE_GRAPH_GBUFFER_H
#define GCT_SHADER_SCENE_GRAPH_GBUFFER_H

#include <gct/primitive_value.h>
#include <gct/gbuffer_value.h>
#include <gct/scene_graph/image_pool.h>
#include <gct/pre_dof_pixel.h>
#include <gct/depth.h>
#include <gct/vertex_attribute.h>
#include <gct/gbuffer_format.h>
#include <gct/color.h>

#ifdef GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

struct gbuffer_image {
  uint gbuffer;
  uint depth;
};

struct gbuffer_iter {
  gbuffer_image image;
  ivec2 image_pos;
  uint active_layer;
};

gbuffer_iter gbuffer_begin(
  gbuffer_image image,
  ivec2 image_pos,
  uint active_layer
) {
  const int layer_count = gbuffer_get_layer_count( active_layer );
  return gbuffer_iter(
    image,
    image_pos,
    active_layer
  );
}

void gbuffer_set(
  gbuffer_iter iter,
  uint image,
  vec4 value
) {
  imageStore( image_pool_2d[ nonuniformEXT( image ) ], iter.image_pos, value );
}

vec4 gbuffer_get(
  gbuffer_iter iter,
  uint image
) {
  return imageLoad( image_pool_2d[ nonuniformEXT( image ) ], iter.image_pos );
}

void gbuffer_set_component(
  gbuffer_iter iter,
  uint layer,
  vec4 value
) {
  if( gbuffer_has_layer( iter.active_layer, layer ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3(
        iter.image_pos,
        gbuffer_get_layer( iter.active_layer, layer )
      ),
      value
    );
  }
}

vec4 gbuffer_get_component(
  gbuffer_iter iter,
  uint layer,
  vec4 default_value
) {
  return
    ( gbuffer_has_layer( iter.active_layer, layer ) ) ?
    imageLoad(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3(
        iter.image_pos,
        gbuffer_get_layer( iter.active_layer, layer )
      )
    ):
    default_value;
}

bool gbuffer_sample_is_nearest(
  gbuffer_iter iter,
  float depth
) {
  const float existing_depth = imageLoad( image_pool_2d[ nonuniformEXT( iter.image.depth ) ], iter.image_pos ).x;
  return depth <= existing_depth;
}

void gbuffer_insert(
  gbuffer_iter iter,
  primitive_value p,
  float depth,
  vec4 input_id
) {
  const int layer_count = gbuffer_get_layer_count( iter.active_layer );
  const bool nearest = gbuffer_sample_is_nearest( iter, depth );
  if( nearest ) {
    imageStore( image_pool_2d[ nonuniformEXT( iter.image.depth ) ], iter.image_pos, vec4( depth, 0.0, 0.0, 0.0 ) );
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_POSITION_DEPTH ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_POSITION_DEPTH )
        ),
        vec4( p.pos.xyz, depth )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_ALBEDO_ALPHA ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_ALBEDO_ALPHA )
        ),
        p.albedo
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_NORMAL ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_NORMAL )
        ),
        vec4( p.normal, input_id.z )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_EMISSIVE_OCCLUSION ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_EMISSIVE_OCCLUSION )
        ),
        vec4( p.emissive, p.occlusion )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_METALLIC_ROUGHNESS_ID ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_METALLIC_ROUGHNESS_ID )
        ),
        vec4( p.metallic, p.roughness, input_id.x, input_id.y )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_OPTFLOW_MARK ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_OPTFLOW_MARK )
        ),
        vec4( p.optflow, 0.0 )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TANGENT ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TANGENT )
        ),
        vec4( p.tangent, input_id.w )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD0_TEXCOORD1 ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD0_TEXCOORD1 )
        ),
        vec4( p.texcoord[ 0 ], 0.0, 1.0 )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD2_TEXCOORD3 ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD2_TEXCOORD3 )
        ),
        vec4( p.texcoord[ 2 ], p.texcoord[ 3 ] )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_COLOR0 ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_COLOR0 )
        ),
        p.color[ 0 ]
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_COLOR1 ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_COLOR0 )
        ),
        p.color[ 1 ]
      );
    }
  }
}

float gbuffer_get_depth(
  gbuffer_iter iter
) {
  return imageLoad( image_pool_2d[ nonuniformEXT( iter.image.depth ) ], iter.image_pos ).x;
}

vec4 gbuffer_get_pos_depth(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_POSITION_DEPTH, vec4( 0, 0, 0, 0 ) );
}

vec4 gbuffer_get_albedo(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_ALBEDO_ALPHA, vec4( 0, 0, 0, 0 ) );
}

vec3 gbuffer_get_normal(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_NORMAL, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 gbuffer_get_normal_meshlet_id(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_NORMAL, vec4( 0, 0, 0, 0 ) );
}

vec3 gbuffer_get_tangent(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_TANGENT, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 gbuffer_get_tangent_face_id(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_TANGENT, vec4( 0, 0, 0, 0 ) );
}

vec4 gbuffer_get_eo(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_EMISSIVE_OCCLUSION, vec4( 0, 0, 0, 1 ) );
}

vec4 gbuffer_get_mrid(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_METALLIC_ROUGHNESS_ID, vec4( 0, 0, 0, 0 ) );
}

vec3 gbuffer_get_optflow(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_OPTFLOW_MARK, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 gbuffer_get_texcoord01(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_TEXCOORD0_TEXCOORD1, vec4( 0, 0, 0, 0 ) );
}

vec4 gbuffer_get_texcoord23(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_TEXCOORD2_TEXCOORD3, vec4( 0, 0, 0, 0 ) );
}

vec4 gbuffer_get_color0(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_COLOR0, vec4( 0, 0, 0, 0 ) );
}

vec4 gbuffer_get_color1(
  gbuffer_iter iter
) {
  return gbuffer_get_component( iter, GCT_GBUFFER_COLOR1, vec4( 0, 0, 0, 0 ) );
}

gbuffer_value gbuffer_get(
  gbuffer_iter iter
) {
  const float depth = imageLoad( image_pool_2d[ nonuniformEXT( iter.image.depth ) ], iter.image_pos ).x;
  const vec3 pos = gbuffer_get_pos_depth( iter ).xyz;
  const vec4 albedo = gbuffer_get_albedo( iter );
  const vec4 normal = gbuffer_get_normal_meshlet_id( iter );
  const vec4 eo = gbuffer_get_eo( iter );
  const vec4 mrid = gbuffer_get_mrid( iter );
  const vec3 optflow = gbuffer_get_optflow( iter );
  const vec4 tangent = gbuffer_get_tangent_face_id( iter );
  const vec4 texcoord01 = gbuffer_get_texcoord01( iter );
  const vec4 texcoord23 = gbuffer_get_texcoord23( iter );
  const vec4 color0 = gbuffer_get_color0( iter );
  const vec4 color1 = gbuffer_get_color1( iter );
  gbuffer_value p;
  p.pos = pos;
  p.depth = depth;
  p.normal = normal.xyz;
  p.metallic = mrid.x;
  p.roughness = mrid.y;
  p.input_id = ivec4( mrid.zw, normal.w, tangent.w );
  p.albedo = albedo;
  p.emissive = eo.xyz;
  p.occlusion = eo.w;
  p.optflow = optflow;
  p.tangent = tangent.xyz;
  p.texcoord[ 0 ] = texcoord01.xy;
  p.texcoord[ 1 ] = texcoord01.zw;
  p.texcoord[ 2 ] = texcoord23.xy;
  p.texcoord[ 3 ] = texcoord23.zw;
  p.color[ 0 ] = color0;
  p.color[ 1 ] = color1;
  return p;
}

#endif

#endif

