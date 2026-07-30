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
bool is_dual_layer( gbuffer_format f ) {
  return std::uint32_t( f ) & std::uint32_t( gbuffer_format::dual_layer );
}
std::uint32_t get_kplus_layer_count( gbuffer_format f ) {
  return 
    ( std::popcount( std::uint32_t( f ) & 0x3FFFFFFF ) * 4u + 1u ) *
    ( is_dual_layer( f ) ? 2u : 1u );
}
std::uint32_t get_ppll_layer_count( gbuffer_format f ) {
  return std::popcount( std::uint32_t( f ) & 0x3FFFFFFF );
}

std::uint32_t get_layer( gbuffer_format active_layers, gbuffer_format layer_bit ) {
  return std::uint32_t( std::popcount( std::uint32_t( active_layers ) & ( ( std::uint32_t( layer_bit ) << 1 ) - 1u ) ) - 1u );
}

bool has_layer( gbuffer_format active_layers, gbuffer_format layer_bit ) {
  return ( std::uint32_t( active_layers ) & std::uint32_t( layer_bit ) ) != 0u;
}

std::uint32_t get_layer_count( gbuffer_format active_layers ) {
  return std::uint32_t( std::popcount( std::uint32_t( active_layers ) & 0x3FFFFFFF ) );
}

std::vector< vk::ImageSubresourceRange > gbuffer_format_to_image_subresource_range( gbuffer_format all, gbuffer_format selected ) {
  std::vector< vk::ImageSubresourceRange > range;
  for( std::uint32_t i = 0u; i != 30u; ++ i )  {
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
  auto set_count = is_dual_layer( all ) ? 8u : 4u;
  for( unsigned int i = 0u; i != set_count; ++i ) {
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
         .setBaseArrayLayer( layer_count * set_count )
         .setLayerCount( is_dual_layer( all ) ? 2u : 1u )
    );
  }
  return kplus_range;
}

std::vector< vk::ImageSubresourceRange > kplus_gbuffer_format_to_image_subresource_range( gbuffer_format all, gbuffer_format selected, bool index, std::uint32_t array_count ) {
  const auto base = kplus_gbuffer_format_to_image_subresource_range( all, selected, index );
  const auto layer_count = get_layer_count( all );
  std::vector< vk::ImageSubresourceRange > range;
  auto set_count = is_dual_layer( all ) ? 8u : 4u;
  auto index_count = is_dual_layer( all ) ? 2u : 1u;
  if( is_dual_layer( all ) ) {
    array_count /= 2u;
  }
  for( std::uint32_t i = 0u; i != array_count; ++i ) {
    for( const auto &b : base ) {
      range.push_back(
        vk::ImageSubresourceRange( b ).setBaseArrayLayer( b.baseArrayLayer + ( layer_count * set_count + index_count ) * i )
      );
    }
  }
  return range;
}

}
