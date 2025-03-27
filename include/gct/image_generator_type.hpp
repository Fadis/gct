#ifndef GCT_IMAGE_GENERATOR_TYPE_HPP
#define GCT_IMAGE_GENERATOR_TYPE_HPP

#include <nlohmann/json_fwd.hpp>

namespace gct {

enum class image_generator_type {
  transfer = 0,
  output = 1,
  inout = 2
};

void to_json( nlohmann::json &dest, const image_generator_type& );

}

#endif

