#ifndef GCT_SUBVIEW_RANGE_HPP
#define GCT_SUBVIEW_RANGE_HPP

#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

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
bool operator==( const subview_range&, const subview_range& );
bool operator!=( const subview_range&, const subview_range& );
void to_json( nlohmann::json&, const subview_range& );
}

#endif

