#ifndef GCT_EOTF_HPP
#define GCT_EOTF_HPP

#include <array>
#include <cstdint>

namespace gct {

float srgb_eotf( float v );
float srgb_oetf( float v );
std::array< float, 4u > srgb_eotf( const std::array< float, 4u > &v );
std::array< float, 4u > srgb_oetf( const std::array< float, 4u > &v );

}
#endif


