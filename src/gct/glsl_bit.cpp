#include <bit>
#include <gct/glsl_bit.hpp>

namespace gct {

std::int8_t bitCount( const std::uint8_t v ) {
  return std::popcount( v );
}
std::int8_t bitCount( const std::int8_t v ) {
  return bitCount( std::uint8_t( v ) );
}

std::int16_t bitCount( const std::uint16_t v ) {
  return std::popcount( v );
}
std::int16_t bitCount( const std::int16_t v ) {
  return bitCount( std::uint16_t( v ) );
}

std::int32_t bitCount( const std::uint32_t v ) {
  return std::popcount( v );
}
std::int32_t bitCount( const std::int32_t v ) {
  return bitCount( std::uint32_t( v ) );
}

std::int64_t bitCount( const std::uint64_t v ) {
  return std::popcount( v );
}
std::int64_t bitCount( const std::int64_t v ) {
  return bitCount( std::uint64_t( v ) );
}

std::int8_t findMSB( const std::uint8_t v ) {
  if( v == 0u ) return -1;
  return  7 - std::countl_zero( v );
}
std::int8_t findMSB( const std::int8_t v ) {
  if( v < 0 ) {
    if( v == -1 ) return -1;
    return 7 - std::countl_one( std::uint8_t( v ) );
  }
  return findMSB( std::uint8_t( v ) );
}

std::int16_t findMSB( const std::uint16_t v ) {
  if( v == 0u ) return -1;
  return  15 - std::countl_zero( v );
}
std::int16_t findMSB( const std::int16_t v ) {
  if( v < 0 ) {
    if( v == -1 ) return -1;
    return 15 - std::countl_one( std::uint16_t( v ) );
  }
  return findMSB( std::uint16_t( v ) );
}

std::int32_t findMSB( const std::uint32_t v ) {
  if( v == 0u ) return -1;
  return  31 - std::countl_zero( v );
}
std::int32_t findMSB( const std::int32_t v ) {
  if( v < 0 ) {
    if( v == -1 ) return -1;
    return 7 - std::countl_one( std::uint32_t( v ) );
  }
  return findMSB( std::uint32_t( v ) );
}

std::int64_t findMSB( const std::uint64_t v ) {
  if( v == 0u ) return -1;
  return  63 - std::countl_zero( v );
}
std::int64_t findMSB( const std::int64_t v ) {
  if( v < 0 ) {
    if( v == -1 ) return -1;
    return 63 - std::countl_one( std::uint64_t( v ) );
  }
  return findMSB( std::uint64_t( v ) );
}

std::int8_t findLSB( const std::uint8_t v ) {
  if( v == 0u ) return -1;
  return  7 - std::countr_zero( v );
}
std::int8_t findLSB( const std::int8_t v ) {
  return findLSB( std::uint8_t( v ) );
}

std::int16_t findLSB( const std::uint16_t v ) {
  if( v == 0u ) return -1;
  return  15 - std::countr_zero( v );
}
std::int16_t findLSB( const std::int16_t v ) {
  return findLSB( std::uint16_t( v ) );
}

std::int32_t findLSB( const std::uint32_t v ) {
  if( v == 0u ) return -1;
  return  31 - std::countr_zero( v );
}
std::int32_t findLSB( const std::int32_t v ) {
  return findLSB( std::uint32_t( v ) );
}

std::int64_t findLSB( const std::uint64_t v ) {
  if( v == 0u ) return -1;
  return  63 - std::countr_zero( v );
}
std::int64_t findLSB( const std::int64_t v ) {
  return findLSB( std::uint64_t( v ) );
}

}
