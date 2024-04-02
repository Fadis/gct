#ifndef GCT_BUFFER_WINDOW_HPP
#define GCT_BUFFER_WINDOW_HPP

#include <cstdint>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct buffer_window_t {
  buffer_window_t() : index( 0 ), offset( 0 ) {}
  LIBGCT_SETTER( index )
  LIBGCT_SETTER( offset )
  std::uint32_t index;
  std::uint32_t offset;
};
void to_json( nlohmann::json&, const buffer_window_t& );
void from_json( const nlohmann::json&, buffer_window_t& );
}

#endif

