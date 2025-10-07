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
}
