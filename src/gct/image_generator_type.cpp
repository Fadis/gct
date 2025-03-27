#include <nlohmann/json.hpp>
#include <gct/image_generator_type.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const image_generator_type &src ) {
  if( src == image_generator_type::inout ) {
    dest = "inout";
  }
  else if( src == image_generator_type::output ) {
    dest = "output";
  }
  else if( src == image_generator_type::transfer ) {
    dest = "transfer";
  }
  else {
    dest = "unknown";
  }
}

}

