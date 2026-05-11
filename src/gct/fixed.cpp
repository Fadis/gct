#include <gct/fixed.hpp>

namespace gct {

float decode_fixed( std::int16_t v ) {
  return float( v ) * 6.103515625e-05f;
}

std::int16_t encode_fixed( float v ) {
  return int( v / 6.103515625e-05 );
}

}

