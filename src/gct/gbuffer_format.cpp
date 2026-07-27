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

std::uint32_t get_layer( gbuffer_format active_layers, gbuffer_format layer_bit ) {
  return std::uint32_t( std::popcount( std::uint32_t( active_layers ) & ( ( std::uint32_t( layer_bit ) << 1 ) - 1u ) ) - 1u );
}

bool has_layer( gbuffer_format active_layers, gbuffer_format layer_bit ) {
  return ( std::uint32_t( active_layers ) & std::uint32_t( layer_bit ) ) != 0u;
}

std::uint32_t get_layer_count( gbuffer_format active_layers ) {
  return std::uint32_t( std::popcount( std::uint32_t( active_layers ) ) );
}

std::vector< vk::ImageSubresourceRange > gbuffer_format_to_image_subresource_range( gbuffer_format all, gbuffer_format selected ) {
  std::vector< vk::ImageSubresourceRange > range;
  for( std::uint32_t i = 0u; i != 32u; ++ i )  {
    std::uint32_t previous_layer = 33u;
    if( has_layer( all, gbuffer_format( 1 << i ) ) && has_layer( selected, gbuffer_format( 1 << i ) ) ) {
      std::uint32_t layer = get_layer( all, gbuffer_format( 1 << i ) );
      if( layer == previous_layer + 1u ) {
        ++range.back().layerCount;
      }
      else {
        range.push_back(
          vk::ImageSubresourceRange()
            .setBaseArrayLayer( layer )
            .setLayerCount( 1u )
        );
      }
      previous_layer = layer;
    }
  }
  return range;
}
std::vector< vk::ImageSubresourceRange > kplus_gbuffer_format_to_image_subresource_range( gbuffer_format all, gbuffer_format selected, bool index ) {
  auto layer_count = get_layer_count( all );
  auto range = gbuffer_format_to_image_subresource_range( all, selected );
  std::vector< vk::ImageSubresourceRange > kplus_range;
  for( unsigned int i = 0u; i != 4u; ++i ) {
    for( auto &r: range ) {
      kplus_range.push_back(
         vk::ImageSubresourceRange()
           .setBaseArrayLayer( r.baseArrayLayer + layer_count * i )
           .setLayerCount( r.layerCount )
      );
    }
  }
  if( index ) {
    kplus_range.push_back(
       vk::ImageSubresourceRange()
         .setBaseArrayLayer( layer_count * 4u )
         .setLayerCount( 1u )
    );
  }
  return kplus_range;
}

}
