#ifndef GCT_SHADER_SCENE_GRAPH_KPLUS_BUFFER_H
#define GCT_SHADER_SCENE_GRAPH_KPLUS_BUFFER_H

#include <gct/primitive_value.h>
#include <gct/gbuffer_value.h>
#include <gct/scene_graph/image_pool.h>
#include <gct/pre_dof_pixel.h>
#include <gct/depth.h>
#include <gct/gbuffer_format.h>


const uint kplus_buffer_array_layer_count = 8;

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

void mark_kplus_buffer(
  uint gbuffer_id,
  ivec2 image_pos
) {
  ivec4 sample_index = ivec4( imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ) ) );
  //if( sample_index.x == 0 ) return;
  const uint top_sample_index = sample_index.x;
  const vec4 existing = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( top_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ) );
  imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_32F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( top_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
}


void mark_kplus_buffer16(
  uint gbuffer_id,
  ivec2 image_pos
) {
  ivec4 sample_index = ivec4( imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, kplus_buffer_array_layer_count * 4u ) ) );
  //if( sample_index.x == 0 ) return;
  const uint top_sample_index = sample_index.x - 1u;
  const vec4 existing = imageLoad( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( top_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ) );
  //imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( top_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
  imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( top_sample_index - 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
  imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( 0 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
  imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( 1 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
  imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( 2 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
  imageStore( GCT_KPLUS_BUFFER_IMAGE_POOL_16F[ nonuniformEXT( gbuffer_id ) ], ivec3( image_pos, ( 3 ) * kplus_buffer_array_layer_count + 5 ), vec4( existing.xyz, existing.w + 1.0 ) );
}

#ifdef GCT_USE_IMAGE_POOL_WITHOUT_FORMAT

struct kplus_image {
  uint gbuffer;
  uint depth;
};

struct kplus_iter {
  kplus_image image;
  ivec2 image_pos;
  int index;
  int layer;
  uint active_layer;
};


kplus_iter kplus_begin(
  kplus_image image,
  ivec2 image_pos,
  uint active_layer
) {
  const ivec4 sample_index = ivec4( imageLoad( image_pool_2d_array[ nonuniformEXT( image.gbuffer ) ], ivec3( image_pos, gbuffer_get_layer_count( active_layer ) * 4u ) ) );
  int index = -1;
  int layer = 0;
  if( sample_index.w != 0u ) {
    index = 3;
    layer = sample_index.w - 1;
  }
  else if( sample_index.z != 0u ) {
    index = 2;
    layer = sample_index.z - 1;
  }
  else if( sample_index.y != 0u ) {
    index = 1;
    layer = sample_index.y - 1;
  }
  else if( sample_index.x != 0u ) {
    index = 0;
    layer = sample_index.x - 1;
  }
  return kplus_iter(
    image,
    image_pos,
    index,
    layer,
    active_layer
  );
}

kplus_iter kplus_begin(
  kplus_image image,
  ivec2 image_pos
) {
  return kplus_begin(
    image,
    image_pos,
    GCT_GBUFFER_POSITION_DEPTH |
    GCT_GBUFFER_ALBEDO_ALPHA |
    GCT_GBUFFER_NORMAL  |
    GCT_GBUFFER_EMISSIVE_OCCLUSION  |
    GCT_GBUFFER_METALLIC_ROUGHNESS_ID |
    GCT_GBUFFER_OPTFLOW_MARK |
    GCT_GBUFFER_SHADOW_LEVEL |
    GCT_GBUFFER_TANGENT
  );
}

kplus_iter kplus_direct(
  kplus_image image,
  ivec2 image_pos,
  int layer,
  uint active_layer
) {
  const ivec4 sample_index = ivec4( imageLoad( image_pool_2d_array[ nonuniformEXT( image.gbuffer ) ], ivec3( image_pos, gbuffer_get_layer_count( active_layer ) * 4u ) ) );
  int index = -1;
  if( sample_index.x - 1 == layer ) {
    index = 0;
  }
  else if( sample_index.y - 1 == layer ) {
    index = 1;
  }
  else if( sample_index.z - 1 == layer ) {
    index = 2;
  }
  else if( sample_index.w - 1 == layer ) {
    index = 3;
  }
  return kplus_iter(
    image,
    image_pos,
    index,
    layer,
    active_layer
  );
}

kplus_iter kplus_direct(
  kplus_image image,
  ivec2 image_pos,
  int layer
) {
  return kplus_direct(
    image,
    image_pos,
    layer,
    GCT_GBUFFER_POSITION_DEPTH |
    GCT_GBUFFER_ALBEDO_ALPHA |
    GCT_GBUFFER_NORMAL  |
    GCT_GBUFFER_EMISSIVE_OCCLUSION  |
    GCT_GBUFFER_METALLIC_ROUGHNESS_ID |
    GCT_GBUFFER_OPTFLOW_MARK |
    GCT_GBUFFER_SHADOW_LEVEL |
    GCT_GBUFFER_TANGENT
  );
}

bool kplus_is_end(
  kplus_iter iter
) {
  return iter.index == -1;
}

kplus_iter kplus_next(
  kplus_iter iter
) {
  const ivec4 sample_index = ivec4( imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ], ivec3( iter.image_pos, gbuffer_get_layer_count( iter.active_layer ) * 4u ) ) );
  iter.index = max( iter.index - 1, -1 );
  if( iter.index == 0 ) {
    iter.layer = sample_index.x - 1;
  }
  else if( iter.index == 1 ) {
    iter.layer = sample_index.y - 1;
  }
  else if( iter.index == 2 ) {
    iter.layer = sample_index.z - 1;
  }
  return iter;
}

bool kplus_equal(
  kplus_iter l,
  kplus_iter r
) {
  return l.index == r.index;
}

ivec3 kplus_get_pixel_pos(
  kplus_iter iter
) {
  return ivec3( iter.image_pos, iter.layer );
}

void kplus_set(
  kplus_iter iter,
  uint image,
  vec4 value
) {
  imageStore( image_pool_2d_array[ nonuniformEXT( image ) ], kplus_get_pixel_pos( iter ), value );
}

vec4 kplus_get(
  kplus_iter iter,
  uint image
) {
  return imageLoad( image_pool_2d_array[ nonuniformEXT( image ) ], kplus_get_pixel_pos( iter ) );
}

void kplus_set_component(
  kplus_iter iter,
  uint layer,
  vec4 value
) {
  if( gbuffer_has_layer( iter.active_layer, layer ) ) {
    imageStore(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3(
        iter.image_pos,
        iter.layer * gbuffer_get_layer_count( iter.active_layer ) +
        gbuffer_get_layer( iter.active_layer, layer )
      ),
      value
    );
  }
}

vec4 kplus_get_component(
  kplus_iter iter,
  uint layer,
  vec4 default_value
) {
  return
    ( gbuffer_has_layer( iter.active_layer, layer ) ) ?
    imageLoad(
      image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
      ivec3(
        iter.image_pos,
        iter.layer * gbuffer_get_layer_count( iter.active_layer ) +
        gbuffer_get_layer( iter.active_layer, layer )
      )
    ):
    default_value;
}



void kplus_insert(
  kplus_iter iter,
  primitive_value p,
  float depth,
  vec2 input_id
) {
  const uint layer_count = gbuffer_get_layer_count( iter.active_layer );
  ivec4 sample_index = ivec4( imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ], ivec3( iter.image_pos, layer_count * 4u ) ) );
  vec4 sample_depth = vec4(
    sample_index.x != 0 ?
      imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], ivec3( iter.image_pos, sample_index.x - 1 ) ).x :
      2.0,
    sample_index.y != 0 ?
      imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], ivec3( iter.image_pos, sample_index.y - 1 ) ).x :
      2.0,
    sample_index.z != 0 ?
      imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], ivec3( iter.image_pos, sample_index.z - 1 ) ).x :
      2.0,
    sample_index.w != 0 ?
      imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], ivec3( iter.image_pos, sample_index.w - 1 ) ).x :
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
    imageStore( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], ivec3( iter.image_pos, new_sample_index - 1 ), vec4( depth, 0.0, 0.0, 0.0 ) );
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_POSITION_DEPTH ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          ( new_sample_index - 1 ) * layer_count +
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
          ( new_sample_index - 1 ) * layer_count +
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
          ( new_sample_index - 1 ) * layer_count +
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_NORMAL )
        ),
        vec4( p.normal, 1.0 )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_EMISSIVE_OCCLUSION ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          ( new_sample_index - 1 ) * layer_count +
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
          ( new_sample_index - 1 ) * layer_count +
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
          ( new_sample_index - 1 ) * layer_count +
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_OPTFLOW_MARK )
        ),
        vec4( p.optflow, 0.0 )
      );
    }
    /*if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_SHADOW_LEVEL ) ) {
    }*/
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TANGENT ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          ( new_sample_index - 1 ) * layer_count +
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TANGENT )
        ),
        vec4( p.tangent, 0.0 )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD0_TEXCOORD1 ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          ( new_sample_index - 1 ) * layer_count +
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD0_TEXCOORD1 )
        ),
        vec4( p.texcoord[ 0 ], p.texcoord[ 1 ] )
      );
    }
    if( gbuffer_has_layer( iter.active_layer, GCT_GBUFFER_TEXCOORD2_TEXCOORD3 ) ) {
      imageStore(
        image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ],
        ivec3(
          iter.image_pos,
          ( new_sample_index - 1 ) * layer_count +
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
          ( new_sample_index - 1 ) * layer_count +
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
          ( new_sample_index - 1 ) * layer_count +
          gbuffer_get_layer( iter.active_layer, GCT_GBUFFER_COLOR0 )
        ),
        p.color[ 1 ]
      );
    }
    imageStore( image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ], ivec3( iter.image_pos, layer_count * 4u ), vec4( sample_index ) );
  }
}

float kplus_get_depth(
  kplus_iter iter
) {
  return imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], kplus_get_pixel_pos( iter ) ).x;
}

vec4 kplus_get_pos_depth(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_POSITION_DEPTH, vec4( 0, 0, 0, 0 ) );
}

vec4 kplus_get_albedo(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_ALBEDO_ALPHA, vec4( 0, 0, 0, 0 ) );
}

vec3 kplus_get_normal(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_NORMAL, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec3 kplus_get_tangent(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_TANGENT, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 kplus_get_eo(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_EMISSIVE_OCCLUSION, vec4( 0, 0, 0, 0 ) );
}

vec4 kplus_get_mrid(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_METALLIC_ROUGHNESS_ID, vec4( 0, 0, 0, 0 ) );
}

vec3 kplus_get_optflow(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_OPTFLOW_MARK, vec4( 0, 0, 0, 0 ) ).xyz;
}

vec4 kplus_get_texcoord01(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_TEXCOORD0_TEXCOORD1, vec4( 0, 0, 0, 0 ) );
}

vec4 kplus_get_texcoord23(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_TEXCOORD2_TEXCOORD3, vec4( 0, 0, 0, 0 ) );
}

vec4 kplus_get_color0(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_COLOR0, vec4( 0, 0, 0, 0 ) );
}

vec4 kplus_get_color1(
  kplus_iter iter
) {
  return kplus_get_component( iter, GCT_GBUFFER_COLOR1, vec4( 0, 0, 0, 0 ) );
}

gbuffer_value kplus_get(
  kplus_iter iter
) {
  const float depth = imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.depth ) ], kplus_get_pixel_pos( iter ) ).x;
  const vec3 pos = kplus_get_pos_depth( iter ).xyz;
  const vec4 albedo = kplus_get_albedo( iter );
  const vec3 normal = kplus_get_normal( iter );
  const vec4 eo = kplus_get_eo( iter );
  const vec4 mrid = kplus_get_mrid( iter );
  const vec3 optflow = kplus_get_optflow( iter );
  const vec3 tangent = kplus_get_tangent( iter );
  const vec4 texcoord01 = kplus_get_texcoord01( iter );
  const vec4 texcoord23 = kplus_get_texcoord23( iter );
  const vec4 color0 = kplus_get_color0( iter );
  const vec4 color1 = kplus_get_color1( iter );
  gbuffer_value p;
  p.pos = pos;
  p.depth = depth;
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

bool kplus_is_nearest(
  kplus_iter iter
) {
  return iter.index == 0;
}


kplus_iter kplus_nearest(
  kplus_iter iter
) {
  iter.index = 0;
  iter.layer = max( int( imageLoad( image_pool_2d_array[ nonuniformEXT( iter.image.gbuffer ) ], ivec3( iter.image_pos, gbuffer_get_layer_count( iter.active_layer ) * 4u ) ).x ) - 1, 0 );
  return iter;
}

kplus_iter kplus_nearest_in_slice(
  kplus_iter iter,
  float depth_threshold
) {
  float depth = kplus_get_depth( iter );
  if( depth_threshold >= depth ) {
    iter.index = -1;
    return iter;
  }
  kplus_iter next = kplus_next( iter ); 
  depth = kplus_get_depth( next );
  for( uint i = 0u; i != 4u; i++ ) {
    if( !kplus_is_end( next ) && depth_threshold < depth ) {
      iter = next;
      next = kplus_next( iter ); 
      depth = kplus_get_depth( next );
    }
  }
  return iter;
}

pre_dof_pixel kplus_mix(
  kplus_iter iter,
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
  for( uint i = 0u; i != 4u; i++ ) {
    const vec4 albedo = kplus_get_albedo( iter );
    const bool has_layer = !kplus_is_end( iter );
    const bool is_nearest = kplus_is_nearest( iter );
    const float depth = has_layer ? 
      decode_depth( kplus_get_depth( iter ), znear, zfar ) :
      zfar;
    const vec3 ambient =
      ( has_layer ) ?
      ambient_factor *
      ( is_nearest ? ao : 1.0 ) *
      albedo.xyz :
      vec3( 0.0, 0.0, 0.0 );
    const vec3 lighting = 
      ( has_layer ) ?
      kplus_get( iter, lighting_image ).xyz :
      vec3( 0.0, 0.0, 0.0 );
    const vec4 scat = 
      ( has_layer ) ?
      kplus_get( iter, scattering_image ) :
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
      iter = kplus_next( iter );
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

#endif

