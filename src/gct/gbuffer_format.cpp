#include <bit>
#include <gct/gbuffer_format.hpp>

namespace gct {
gbuffer_format operator|( gbuffer_format l, gbuffer_format r ) {
  return gbuffer_format( std::uint32_t( l ) | std::uint32_t( r ) );
}
gbuffer_format operator&( gbuffer_format l, gbuffer_format r ) {
  return gbuffer_format( std::uint32_t( l ) & std::uint32_t( r ) );
}
gbuffer_format operator^( gbuffer_format l, gbuffer_format r ) {
  return gbuffer_format( std::uint32_t( l ) ^ std::uint32_t( r ) );
}
std::uint32_t get_kplus_layer_count( gbuffer_format f ) {
  return std::popcount( std::uint32_t( f ) ) * 4u + 1u;
}
std::uint32_t get_ppll_layer_count( gbuffer_format f ) {
  return std::popcount( std::uint32_t( f ) );
}
}
