#include "gct/html_color.hpp"

namespace gct {

std::array< float, 4u > html_color( std::uint32_t v ) {
  return std::array< float, 4u >{
    float( ( v >> 16 ) & 0xFFu )/255.f,
    float( ( v >> 8 ) & 0xFFu )/255.f,
    float( v & 0xFFu )/255.f,
    1.f
  };
}

}

