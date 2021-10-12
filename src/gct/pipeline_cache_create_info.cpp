#include <fstream>
#include <iterator>
#include <gct/surface.hpp>
#include <gct/pipeline_cache_create_info.hpp>

namespace gct {
  pipeline_cache_create_info_t &pipeline_cache_create_info_t::rebuild_chain() {
    if( data.empty() )
      basic
        .setInitialDataSize( 0u )
        .setPInitialData( nullptr );
    else
      basic
        .setInitialDataSize( data.size() )
        .setPInitialData( reinterpret_cast< const void* >( data.data() ) );
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_cache_create_info_t &pipeline_cache_create_info_t::load( const std::string &filename ) {
    std::fstream stream( filename, std::ios::binary|std::ios::in );
    data.assign(
      std::istreambuf_iterator< char >( stream ),
      std::istreambuf_iterator< char >()
    );
    chained = false;
    return *this;
  }
}

