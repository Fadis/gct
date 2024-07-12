#ifndef GCT_PUNCTUAL_LIGHT_TYPE_HPP
#define GCT_PUNCTUAL_LIGHT_TYPE_HPP
#include <cstdint>
#include <nlohmann/json_fwd.hpp>

namespace gct {

enum class punctual_light_type : std::uint32_t {
  directional = 0u,
  point = 1u,
  spot = 2u
};

void to_json( nlohmann::json &dest, const punctual_light_type &src );
void from_json( const nlohmann::json &src, punctual_light_type &dest );

}

#endif

