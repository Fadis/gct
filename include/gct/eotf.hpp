#ifndef GCT_EOTF_HPP
#define GCT_EOTF_HPP

#include <array>
#include <cstdint>

namespace gct {

[[nodiscard]] float srgb_eotf( float v );
[[nodiscard]] float srgb_oetf( float v );
[[nodiscard]] std::array< float, 4u > srgb_eotf( const std::array< float, 4u > &v );
[[nodiscard]] std::array< float, 4u > srgb_oetf( const std::array< float, 4u > &v );

}
#endif


