#include <fstream>
#include <iterator>
#include <gct/shader_module_create_info.hpp>
#include <vulkan2json/ShaderModuleCreateInfo.hpp>
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
#include <vulkan2json/ShaderModuleValidationCacheCreateInfoEXT.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const shader_module_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( validation_cache ) 
#endif
  }
  void from_json( const nlohmann::json &root, shader_module_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to shader_module_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( validation_cache ) 
#endif
  }
  shader_module_create_info_t &shader_module_create_info_t::rebuild_chain() {
    basic.setCodeSize( code.size() );
    basic.setPCode( reinterpret_cast< const uint32_t* >( code.data() ) );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( validation_cache ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  shader_module_create_info_t &shader_module_create_info_t::load( const std::string &filename ) {
    std::fstream file( filename, std::ios::in|std::ios::binary );
    if( !file.good() ) throw -1;
    code.assign( ( std::istreambuf_iterator< char >( file ) ), std::istreambuf_iterator<char>() );
    return *this;
  }
}

