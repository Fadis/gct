#ifndef GCT_FIXED_H
#define GCT_FIXED_H

#include <cstdint>

namespace gct {

float decode_fixed( std::int16_t v );
std::int16_t encode_fixed( float v );

}

#endif


