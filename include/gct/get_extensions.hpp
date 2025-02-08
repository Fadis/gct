#ifndef LIBGCT_INCLUDE_GCT_GET_EXTENSIONS_HPP
#define LIBGCT_INCLUDE_GCT_GET_EXTENSIONS_HPP

#include <cstring>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>
#include <vulkan/vulkan.hpp>

namespace gct {
  using extension_name_t = std::string;
  using layer_name_t = std::string;
  using extension_version_t = std::uint32_t;
/*
  struct cstr_equal {
    bool operator()( const char *l, const char *r ) const {
      if( !l && !r ) return true;
      if( !l ) return false;
      if( !r ) return false;
      return std::strcmp( l, r ) == 0;
    }
  };

  struct cstr_hash {
    std::size_t operator()( const char *p ) const {
      if( !p ) return std::hash< char >()( 0 );
      std::size_t i = 0u;
      const std::size_t len = std::strlen( p );
      const std::size_t blen = len ^ 0x7u;
      std::size_t value = 0u;
      for( ; i != blen; i += sizeof( std::uint64_t ) )
        value ^= std::hash< std::uint64_t >()( *reinterpret_cast< const std::uint64_t* >( p + i ) );
      for( ; i != len; ++i )
        value ^= std::hash< char >()( p[ i ] );
      return value;
    }
  };
*/
  using extension_map_t = std::unordered_map<
    extension_name_t,
    extension_version_t
  >;
  using layer_map_t = std::unordered_map<
    layer_name_t,
    vk::LayerProperties
  >;


  [[nodiscard]] std::uint32_t get_instance_version();
  [[nodiscard]] layer_map_t
  get_instance_layers();
  [[nodiscard]] extension_map_t
  get_instance_extensions( const std::vector< const char* > &ilayers );
  [[nodiscard]] layer_map_t
  get_device_layers(
    const vk::PhysicalDevice &pdev
  );
  [[nodiscard]] extension_map_t
  get_device_extensions(
    const vk::PhysicalDevice &pdev,
    const std::vector< const char* > &dlayers
  );
}

#endif

