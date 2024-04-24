#ifndef GCT_KEY_VALUE_HPP
#define GCT_KEY_VALUE_HPP
#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {

struct key_value_t {
  LIBGCT_SETTER( key )
  LIBGCT_SETTER( value )
  std::uint32_t key = 0u;
  std::uint32_t value = 0u;
};

void to_json( nlohmann::json&, const key_value_t& );
void from_json( const nlohmann::json&, key_value_t& );

}

#endif

