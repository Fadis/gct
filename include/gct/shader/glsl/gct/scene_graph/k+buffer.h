#ifndef GCT_SHADER_SCENE_GRAPH_KPLUS_BUFFER_H
#define GCT_SHADER_SCENE_GRAPH_KPLUS_BUFFER_H

#include <gct/primitive_value.h>
#include <gct/gbuffer_value.h>
#include <gct/scene_graph/image_pool.h>

uint kplus_buffer_array_layer_count = 8;

#ifdef GCT_USE_IMAGE_POOL_WITHOUT_FORMAT
#define GCT_KPLUS_BUFFER_IMAGE_POOL_R32F image_pool_2d_array
#define GCT_KPLUS_BUFFER_IMAGE_POOL_32F image_pool_2d_array
#define GCT_KPLUS_BUFFER_IMAGE_POOL_16F image_pool_2d_array
#else
#define GCT_KPLUS_BUFFER_IMAGE_POOL_R32F image_poolr32f_array
#define GCT_KPLUS_BUFFER_IMAGE_POOL_32F image_pool32f_array
#define GCT_KPLUS_BUFFER_IMAGE_POOL_16F image_pool16f_array
#endif

bool update_kplus_buffer(
  uint gbuffer_id,
  uint depth_id,
  ivec2 image_pos,
  primitive_value p,
  float depth,
  vec2 input_id,
  vec4 shadow_level
) {
  ivec4 sample_index = ivec4( imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ) ) );
  vec4 sample_depth = vec4(
    sample_index.x != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.x - 1 ) ).x :
      2.0,
    sample_index.y != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.y - 1 ) ).x :
      2.0,
    sample_index.z != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.z - 1 ) ).x :
      2.0,
    sample_index.w != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.w - 1 ) ).x :
      2.0
  );
  const uint existing_sample_count =
    ( sample_index.x != 0 ? 1 : 0 ) +
    ( sample_index.y != 0 ? 1 : 0 ) +
    ( sample_index.z != 0 ? 1 : 0 ) +
    ( sample_index.w != 0 ? 1 : 0 );
  uint new_sample_index =
    ( existing_sample_count >= 4 ) ?
      uint( sample_index.w ) :
      existing_sample_count + 1;
  const uint new_sample_pos =
    ( depth >= sample_depth.x ? 1 : 0 ) +
    ( depth >= sample_depth.y ? 1 : 0 ) +
    ( depth >= sample_depth.z ? 1 : 0 ) +
    ( depth >= sample_depth.w ? 1 : 0 );
  sample_index = (
    new_sample_pos == 0 ?
    ivec4( new_sample_index, sample_index.x, sample_index.y, sample_index.z ) :
    (
      new_sample_pos == 1 ?
      ivec4( sample_index.x, new_sample_index, sample_index.y, sample_index.z ) :
      (
        new_sample_pos == 2 ?
        ivec4( sample_index.x, sample_index.y, new_sample_index, sample_index.z ) :
        (
          new_sample_pos == 3 ?
          ivec4( sample_index.x, sample_index.y, sample_index.z, new_sample_index ) :
          ivec4( sample_index.x, sample_index.y, sample_index.z, sample_index.w )
        )
      )
    )
  );
  if( new_sample_pos < 4 ) {
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, new_sample_index - 1 ), vec4( depth, 0.0, 0.0, 0.0 ) );
    //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 0 ), vec4( p.pos, depth ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 1 ), p.albedo );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 2 ), vec4( p.normal.x, p.normal.y, p.normal.z, 1.0 ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 3 ), vec4( p.emissive, p.occlusion ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 4 ), vec4( p.metallic, p.roughness, input_id.x, input_id.y ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( p.optflow, 0.0 ) );
    //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 6 ), shadow_level );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ), vec4( sample_index ) );
  }
  return new_sample_pos < 4;
}

bool update_kplus_buffer16(
  uint gbuffer_id,
  uint depth_id,
  ivec2 image_pos,
  primitive_value p,
  float depth,
  vec2 input_id,
  vec4 shadow_level
) {
  ivec4 sample_index = ivec4( imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ) ) );
  vec4 sample_depth = vec4(
    sample_index.x != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.x - 1 ) ).x :
      2.0,
    sample_index.y != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.y - 1 ) ).x :
      2.0,
    sample_index.z != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.z - 1 ) ).x :
      2.0,
    sample_index.w != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.w - 1 ) ).x :
      2.0
  );
  const uint existing_sample_count =
    ( sample_index.x != 0 ? 1 : 0 ) +
    ( sample_index.y != 0 ? 1 : 0 ) +
    ( sample_index.z != 0 ? 1 : 0 ) +
    ( sample_index.w != 0 ? 1 : 0 );
  uint new_sample_index =
    ( existing_sample_count >= 4 ) ?
      uint( sample_index.w ) :
      existing_sample_count + 1;
  const uint new_sample_pos =
    ( depth >= sample_depth.x ? 1 : 0 ) +
    ( depth >= sample_depth.y ? 1 : 0 ) +
    ( depth >= sample_depth.z ? 1 : 0 ) +
    ( depth >= sample_depth.w ? 1 : 0 );
  sample_index = (
    new_sample_pos == 0 ?
    ivec4( new_sample_index, sample_index.x, sample_index.y, sample_index.z ) :
    (
      new_sample_pos == 1 ?
      ivec4( sample_index.x, new_sample_index, sample_index.y, sample_index.z ) :
      (
        new_sample_pos == 2 ?
        ivec4( sample_index.x, sample_index.y, new_sample_index, sample_index.z ) :
        (
          new_sample_pos == 3 ?
          ivec4( sample_index.x, sample_index.y, sample_index.z, new_sample_index ) :
          ivec4( sample_index.x, sample_index.y, sample_index.z, sample_index.w )
        )
      )
    )
  );
  if( new_sample_pos < 4 ) {
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, new_sample_index - 1 ), vec4( depth, 0.0, 0.0, 0.0 ) );
    //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 0 ), vec4( p.pos, depth ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 1 ), p.albedo );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 2 ), vec4( p.normal.x, p.normal.y, p.normal.z, 1.0 ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 3 ), vec4( p.emissive, p.occlusion ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 4 ), vec4( p.metallic, p.roughness, input_id.x, input_id.y ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( p.optflow, 0.0 ) );
    //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 6 ), shadow_level );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ), vec4( sample_index ) );
  }
  return new_sample_pos < 4;
}

bool update_kplus_buffer_reduced(
  uint gbuffer_id,
  uint depth_id,
  ivec2 image_pos,
  primitive_value p,
  float depth,
  vec2 input_id,
  vec4 shadow_level
) {
  ivec4 sample_index = ivec4( imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ) ) );
  vec4 sample_depth = vec4(
    sample_index.x != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.x - 1 ) ).x :
      2.0,
    sample_index.y != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.y - 1 ) ).x :
      2.0,
    sample_index.z != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.z - 1 ) ).x :
      2.0,
    sample_index.w != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.w - 1 ) ).x :
      2.0
  );
  const uint existing_sample_count =
    ( sample_index.x != 0 ? 1 : 0 ) +
    ( sample_index.y != 0 ? 1 : 0 ) +
    ( sample_index.z != 0 ? 1 : 0 ) +
    ( sample_index.w != 0 ? 1 : 0 );
  uint new_sample_index =
    ( existing_sample_count >= 4 ) ?
      uint( sample_index.w ) :
      existing_sample_count + 1;
  const uint new_sample_pos =
    ( depth >= sample_depth.x ? 1 : 0 ) +
    ( depth >= sample_depth.y ? 1 : 0 ) +
    ( depth >= sample_depth.z ? 1 : 0 ) +
    ( depth >= sample_depth.w ? 1 : 0 );
  sample_index = (
    new_sample_pos == 0 ?
    ivec4( new_sample_index, sample_index.x, sample_index.y, sample_index.z ) :
    (
      new_sample_pos == 1 ?
      ivec4( sample_index.x, new_sample_index, sample_index.y, sample_index.z ) :
      (
        new_sample_pos == 2 ?
        ivec4( sample_index.x, sample_index.y, new_sample_index, sample_index.z ) :
        (
          new_sample_pos == 3 ?
          ivec4( sample_index.x, sample_index.y, sample_index.z, new_sample_index ) :
          ivec4( sample_index.x, sample_index.y, sample_index.z, sample_index.w )
        )
      )
    )
  );
  if( new_sample_pos < 4 ) {
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, new_sample_index - 1 ), vec4( depth, 0.0, 0.0, 0.0 ) );
    //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 0 ), vec4( p.pos, depth ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 2 ), vec4( p.normal.x, p.normal.y, p.normal.z, 1.0 ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 4 ), vec4( p.metallic, p.roughness, input_id.x, input_id.y ) );
    if( ( ( image_pos.x & 0x1 ) == 0 ) && ( ( image_pos.y & 0x1 ) == 0 ) ) {
      imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 1 ), p.albedo );
      imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 3 ), vec4( p.emissive, p.occlusion ) );
      imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( p.optflow, 0.0 ) );
      //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 6 ), shadow_level );
    }
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ), vec4( sample_index ) );
  }
  return new_sample_pos < 4;
}

bool update_kplus_buffer16_reduced(
  uint gbuffer_id,
  uint depth_id,
  ivec2 image_pos,
  primitive_value p,
  float depth,
  vec2 input_id,
  vec4 shadow_level
) {
  ivec4 sample_index = ivec4( imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ) ) );
  vec4 sample_depth = vec4(
    sample_index.x != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.x - 1 ) ).x :
      2.0,
    sample_index.y != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.y - 1 ) ).x :
      2.0,
    sample_index.z != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.z - 1 ) ).x :
      2.0,
    sample_index.w != 0 ?
      imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, sample_index.w - 1 ) ).x :
      2.0
  );
  const uint existing_sample_count =
    ( sample_index.x != 0 ? 1 : 0 ) +
    ( sample_index.y != 0 ? 1 : 0 ) +
    ( sample_index.z != 0 ? 1 : 0 ) +
    ( sample_index.w != 0 ? 1 : 0 );
  uint new_sample_index =
    ( existing_sample_count >= 4 ) ?
      uint( sample_index.w ) :
      existing_sample_count + 1;
  const uint new_sample_pos =
    ( depth >= sample_depth.x ? 1 : 0 ) +
    ( depth >= sample_depth.y ? 1 : 0 ) +
    ( depth >= sample_depth.z ? 1 : 0 ) +
    ( depth >= sample_depth.w ? 1 : 0 );
  sample_index = (
    new_sample_pos == 0 ?
    ivec4( new_sample_index, sample_index.x, sample_index.y, sample_index.z ) :
    (
      new_sample_pos == 1 ?
      ivec4( sample_index.x, new_sample_index, sample_index.y, sample_index.z ) :
      (
        new_sample_pos == 2 ?
        ivec4( sample_index.x, sample_index.y, new_sample_index, sample_index.z ) :
        (
          new_sample_pos == 3 ?
          ivec4( sample_index.x, sample_index.y, sample_index.z, new_sample_index ) :
          ivec4( sample_index.x, sample_index.y, sample_index.z, sample_index.w )
        )
      )
    )
  );
  if( new_sample_pos < 4 ) {
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( image_pos, new_sample_index - 1 ), vec4( depth, 0.0, 0.0, 0.0 ) );
    //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 0 ), vec4( p.pos, depth ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 2 ), vec4( p.normal.x, p.normal.y, p.normal.z, 1.0 ) );
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 4 ), vec4( p.metallic, p.roughness, input_id.x, input_id.y ) );
    if( ( ( image_pos.x & 0x1 ) == 0 ) && ( ( image_pos.y & 0x1 ) == 0 ) ) {
      imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 1 ), p.albedo );
      imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 3 ), vec4( p.emissive, p.occlusion ) );
      imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( p.optflow, 0.0 ) );
      //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( new_sample_index - 1 ) * kplus_buffer_array_layer_count + 6 ), shadow_level );
    }
    imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ), vec4( sample_index ) );
  }
  return new_sample_pos < 4;
}




gbuffer_value read_kplus_buffer(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  const vec4 pos = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 0 ) );
  const vec4 albedo = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 1 ) ).rgba;
  const vec3 normal = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 2 ) ).xyz;
  const vec4 eo = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 3 ) );
  const vec4 mrid = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 4 ) );
  const vec3 optflow = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 5 ) ).rgb;
  const vec4 shadow_level = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 6 ) ); 
  gbuffer_value p;
  p.pos = pos.xyz;
  p.depth = pos.w;
  p.normal = normal;
  p.metallic = mrid.x;
  p.roughness = mrid.y;
  p.input_id = ivec2( mrid.zw );
  p.albedo = albedo;
  p.emissive = eo.xyz;
  p.occlusion = eo.w;
  p.optflow = optflow;
  p.shadow_level = shadow_level;
  return p;
}

gbuffer_value read_kplus_buffer16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  const vec4 pos = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 0 ) );
  const vec4 albedo = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 1 ) ).rgba;
  const vec3 normal = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 2 ) ).xyz;
  const vec4 eo = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 3 ) );
  const vec4 mrid = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 4 ) );
  const vec3 optflow = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 5 ) ).rgb;
  const vec4 shadow_level = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 6 ) ); 
  gbuffer_value p;
  p.pos = pos.xyz;
  p.depth = pos.w;
  p.normal = normal;
  p.metallic = mrid.x;
  p.roughness = mrid.y;
  p.input_id = ivec2( mrid.zw );
  p.albedo = albedo;
  p.emissive = eo.xyz;
  p.occlusion = eo.w;
  p.optflow = optflow;
  p.shadow_level = shadow_level;
  return p;
}

float read_kplus_buffer_depth(
  uint depth_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_R32F[ nonuniformEXT( depth_id ) ], ivec3( screen_pos, layer ) ).x;
}

vec4 read_kplus_buffer_pos_depth(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 0 ) );
}

vec4 read_kplus_buffer_albedo(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 1 ) );
}

vec4 read_kplus_buffer_normal(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 2 ) );
}

vec4 read_kplus_buffer_eo(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 3 ) );
}

vec4 read_kplus_buffer_mrid(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 4 ) );
}

vec4 read_kplus_buffer_optflow(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 5 ) );
}

vec4 read_kplus_buffer_shadow_level(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 6 ) );
}

vec4 read_kplus_buffer_pos_depth16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 0 ) );
}

vec4 read_kplus_buffer_albedo16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 1 ) );
}

vec4 read_kplus_buffer_normal16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 2 ) );
}

vec4 read_kplus_buffer_eo16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 3 ) );
}

vec4 read_kplus_buffer_mrid16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 4 ) );
}

vec4 read_kplus_buffer_optflow16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 5 ) );
}

vec4 read_kplus_buffer_shadow_level16(
  uint gbuffer_id,
  ivec2 screen_pos,
  uint layer
) {
  return imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( screen_pos, layer * kplus_buffer_array_layer_count + 6 ) );
}

#endif

