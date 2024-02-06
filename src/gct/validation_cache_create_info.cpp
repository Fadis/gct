#include <vulkan/vulkan.hpp>
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <gct/validation_cache_create_info.hpp>
#include <vulkan2json/ValidationCacheCreateInfoEXT.hpp>

namespace gct {
  void to_json( nlohmann::json &root, const validation_cache_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    if( !v.get_data().empty() )
      root[ "data" ] = v.get_data();
  }
  void from_json( const nlohmann::json &root, validation_cache_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to validation_cache_create_info_t", __FILE__, __LINE__ );
    if( root.find( "data" ) == root.end() ) {
      LIBGCT_EXTENSION_FROM_JSON( basic )
    }
    else {
      v.load_from_memory( std::vector< std::uint8_t >( root[ "data" ] ) );
    }
  }
  validation_cache_create_info_t &validation_cache_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( !data.empty() ) {
      basic.setInitialDataSize( data.size() );
      basic.setPInitialData( reinterpret_cast< const uint32_t* >( data.data() ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  validation_cache_create_info_t &validation_cache_create_info_t::load( const std::string &filename ) {
    std::fstream file( filename, std::ios::in|std::ios::binary );
    if( !file.good() ) throw -1;
    std::vector< std::uint8_t > temp( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator<char>() );
    data = std::move( temp );
    return *this;
  }
  validation_cache_create_info_t &validation_cache_create_info_t::load_from_memory( const std::vector< std::uint8_t > &v ) {
    data = v;
    return *this;
  }
}
#endif

