#include <cmath>
#include "gct/eotf.hpp"

namespace gct {

float srgb_eotf( float v ) {
  if( v < 0.0031308f ) return 12.92f * v;
  else return 1.055f * std::pow( v, 1.f/2.4f ) - 0.055f;
}
float srgb_oetf( float v ) {
  if( v < 0.04045f ) return 1.f/12.92f * v;
  else return std::pow( ( v + 0.055f ) / 1.055f, 2.4f );
}

std::array< float, 4u > srgb_eotf( const std::array< float, 4u > &v ) {
  return std::array< float, 4u >{
    srgb_eotf( v[ 0 ] ),
    srgb_eotf( v[ 1 ] ),
    srgb_eotf( v[ 2 ] ),
    v[ 3 ]
  };
}
std::array< float, 4u > srgb_oetf( const std::array< float, 4u > &v ) {
  return std::array< float, 4u >{
    srgb_oetf( v[ 0 ] ),
    srgb_oetf( v[ 1 ] ),
    srgb_oetf( v[ 2 ] ),
    v[ 3 ]
  };
}

}

