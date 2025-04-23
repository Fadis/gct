#ifndef GCT_NUMERIC_TYPES_HPP
#define GCT_NUMERIC_TYPES_HPP

#include <nlohmann/json_fwd.hpp>

namespace gct {

enum class type_id {
  float_ = 0,
  u8 = 1,
  u16 = 2,
  u32 = 3
};

void to_json( nlohmann::json &dest, const type_id &src );

}

#endif

