#ifndef GCT_GLSL_BIT_HPP
#define GCT_GLSL_BIT_HPP
#include <cstdint>

namespace gct {

std::int8_t bitCount( const std::uint8_t v );
std::int8_t bitCount( const std::int8_t v );
std::int16_t bitCount( const std::uint16_t v );
std::int16_t bitCount( const std::int16_t v );
std::int32_t bitCount( const std::uint32_t v );
std::int32_t bitCount( const std::int32_t v );
std::int64_t bitCount( const std::uint64_t v );
std::int64_t bitCount( const std::int64_t v );
std::int8_t findMSB( const std::uint8_t v );
std::int8_t findMSB( const std::int8_t v );
std::int16_t findMSB( const std::uint16_t v );
std::int16_t findMSB( const std::int16_t v );
std::int32_t findMSB( const std::uint32_t v );
std::int32_t findMSB( const std::int32_t v );
std::int64_t findMSB( const std::uint64_t v );
std::int64_t findMSB( const std::int64_t v );
std::int8_t findLSB( const std::uint8_t v );
std::int8_t findLSB( const std::int8_t v );
std::int16_t findLSB( const std::uint16_t v );
std::int16_t findLSB( const std::int16_t v );
std::int32_t findLSB( const std::uint32_t v );
std::int32_t findLSB( const std::int32_t v );
std::int64_t findLSB( const std::uint64_t v );
std::int64_t findLSB( const std::int64_t v );

template< typename T >
T bitfieldReverse( T value ) {
  T reversed = 0u;
  for( unsigned int i = 0u; i != sizeof( T ) * 8u; ++i ) {
    reversed <<= 1;
    reversed |= value & 0x1;
    value >>= 1;
  }
  return reversed;
}

}

#endif

