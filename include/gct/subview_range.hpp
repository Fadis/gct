#ifndef GCT_SUBVIEW_RANGE_HPP
#define GCT_SUBVIEW_RANGE_HPP

namespace gct {
  struct subview_range {
    LIBGCT_SETTER( mip_offset )
    LIBGCT_SETTER( mip_count )
    LIBGCT_SETTER( layer_offset )
    LIBGCT_SETTER( layer_count )
    LIBGCT_SETTER( force_array )
    std::uint32_t mip_offset = 0u;
    std::uint32_t mip_count = 1u;
    std::uint32_t layer_offset = 0u;
    std::uint32_t layer_count = 1u;
    bool force_array = false;
  };
}

#endif

