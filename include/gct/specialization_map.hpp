#ifndef GCT_SPECIALIZATION_MAP_HPP
#define GCT_SPECIALIZATION_MAP_HPP

#include <cstdint>
#include <utility>
#include <unordered_map>
#include <variant>
#include <gct/specialization_info.hpp>

namespace gct {
  struct specialization_value_type {
    using value_type = std::variant<
      std::uint32_t,
      float,
      double
    >;
    specialization_value_type( bool v ) : value( std::uint32_t( v ) ) {
    }
    specialization_value_type( std::int32_t v ) : value( std::uint32_t( v ) ) {
    }
    specialization_value_type( std::uint32_t v ) : value( v ) {
    }
    specialization_value_type( float v ) : value( v ) {
    }
    specialization_value_type( double v ) : value( v ) {
    }
    value_type value;
  };
  using specialization_map_item = std::pair< std::uint32_t, specialization_value_type >;
  using specialization_map = std::unordered_map< std::uint32_t, specialization_value_type >;
  encoded_specialization_info_t encode( const specialization_map & );
}

#endif
