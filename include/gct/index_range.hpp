#ifndef GCT_INDEX_RANGE_HPP
#define GCT_INDEX_RANGE_HPP

#include <cstdint>
#include <type_traits>
#include <utility>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct index_range {
  LIBGCT_SETTER( offset )
  LIBGCT_SETTER( count )
  std::uint32_t offset = 0u;
  std::uint32_t count = 0u;
};

void to_josn( nlohmann::json&, const index_range& );
void from_josn( const nlohmann::json&, index_range& );

}

#endif

