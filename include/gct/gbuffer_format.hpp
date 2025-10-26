#ifndef GCT_GBUFFER_FORMAT_HPP
#define GCT_GBUFFER_FORMAT_HPP

#include <cstdint>

namespace gct {
  enum class gbuffer_format : std::uint32_t {
    position_depth = ( 1u << 0 ),
    albedo_alpha = ( 1u << 1 ),
    normal = ( 1u << 2 ),
    emissive_occlusion = ( 1u << 3 ),
    metallic_roughness_id = ( 1u << 4 ),
    optflow_mark = ( 1u << 5 ),
    shadow_level = ( 1u << 6 ),
    tangent = ( 1u << 7 ),
    texcoord0_texcoord1 = ( 1u << 8 ),
    texcoord2_texcoord3 = ( 1u << 9 ),
    color0 = ( 1u << 10 ),
    color1 = ( 1u << 11 )
  };
  gbuffer_format operator|( gbuffer_format l, gbuffer_format r );
  gbuffer_format operator&( gbuffer_format l, gbuffer_format r );
  gbuffer_format operator^( gbuffer_format l, gbuffer_format r );
  std::uint32_t get_kplus_layer_count( gbuffer_format );
  std::uint32_t get_ppll_layer_count( gbuffer_format );
}

#endif

