#ifndef GCT_SHADER_GBUFFER_FORMAT_H
#define GCT_SHADER_GBUFFER_FORMAT_H

#define GCT_GBUFFER_POSITION_DEPTH ( 1u << 0 )
#define GCT_GBUFFER_ALBEDO_ALPHA   ( 1u << 1 )
#define GCT_GBUFFER_NORMAL   ( 1u << 2 )
#define GCT_GBUFFER_EMISSIVE_OCCLUSION     ( 1u << 3 )
#define GCT_GBUFFER_METALLIC_ROUGHNESS_ID ( 1u << 4 )
#define GCT_GBUFFER_OPTFLOW_MARK ( 1u << 5 )
#define GCT_GBUFFER_SHADOW_LEVEL ( 1u << 6 )
#define GCT_GBUFFER_TANGENT ( 1u << 7 )
#define GCT_GBUFFER_TEXCOORD0_TEXCOORD1 ( 1u << 8 )
#define GCT_GBUFFER_TEXCOORD2_TEXCOORD3 ( 1u << 9 )
#define GCT_GBUFFER_COLOR0   ( 1u << 10 )
#define GCT_GBUFFER_COLOR1   ( 1u << 11 )

uint gbuffer_get_layer( uint active_layers, uint layer_bit ) {
  return bitCount( active_layers & ( ( layer_bit << 1 ) - 1u ) ) - 1u;
}

bool gbuffer_has_layer( uint active_layers, uint layer_bit ) {
  return ( active_layers & layer_bit ) != 0u;
}

uint gbuffer_get_layer_count( uint active_layers ) {
  return bitCount( active_layers );
}

#endif

