#include "gct/exception.hpp"
#include <fstream>
#include <iterator>
#include <nlohmann/json.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/shader_module_reflection.hpp>
#include <vulkan2json/ShaderModuleCreateInfo.hpp>
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
#include <vulkan2json/ShaderModuleValidationCacheCreateInfoEXT.hpp>
#endif
#include <gct/spv2json.hpp>
#include <gct/spirv_reflect.h>

namespace gct {
  void to_json( nlohmann::json &root, const shader_module_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_cache ) 
#endif
    if( !v.get_code().empty() )
      root[ "code" ] = v.get_code();
    if( v.has_reflection() )
      root[ "reflection" ] = *v.get_reflection();
  }
  void from_json( const nlohmann::json &root, shader_module_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to shader_module_create_info_t", __FILE__, __LINE__ );
    if( root.find( "code" ) == root.end() ) {
      LIBGCT_EXTENSION_FROM_JSON( basic )
    }
    else {
      v.load_from_memory( std::vector< std::uint8_t >( root[ "code" ] ) );
    }
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( validation_cache ) 
#endif
  }
  shader_module_create_info_t &shader_module_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( !code.empty() ) {
      basic.setCodeSize( code.size() );
      basic.setPCode( reinterpret_cast< const uint32_t* >( code.data() ) );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_cache ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  shader_module_create_info_t &shader_module_create_info_t::load( const std::string &filename ) {
    if( !std::filesystem::exists( std::filesystem::path( filename ) ) ) {
      throw exception::invalid_argument( std::string( "shader_module_create_info_t::load : File not found : " ) + filename, __FILE__, __LINE__ ); 
    }
    std::fstream file( filename, std::ios::in|std::ios::binary );
    if( !file.good() ) {
      throw exception::invalid_argument( std::string( "shader_module_create_info_t::load : Unable to open file : " ) + filename, __FILE__, __LINE__ );
    }
    std::vector< std::uint8_t > temp( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator<char>() );
    auto raw = new shader_module_reflection_t( temp );
    chained = false;
    reflect.reset( raw );
    code = std::move( temp );
    return *this;
  }
  shader_module_create_info_t &shader_module_create_info_t::load_from_memory( const std::vector< std::uint8_t > &v ) {
    std::vector< std::uint8_t > temp = v;
    auto raw = new shader_module_reflection_t( temp );
    chained = false;
    reflect.reset( raw );
    code = std::move( temp );
    return *this;
  }
  const shader_module_reflection_t &shader_module_create_info_t::get_reflection() const {
    if( !reflect ) throw exception::invalid_argument( "Reflection is not available", __FILE__, __LINE__ );
    return *reflect;
  }
}

