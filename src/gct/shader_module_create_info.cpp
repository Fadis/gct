#include <fstream>
#include <iterator>
#include <gct/shader_module_create_info.hpp>

namespace gct {
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

