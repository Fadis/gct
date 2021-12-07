#include <fstream>
#include <iterator>
#include <vulkan2json/PipelineCacheCreateInfo.hpp>
#include <gct/surface.hpp>
#include <gct/pipeline_cache_create_info.hpp>

namespace gct {

  void to_json( nlohmann::json &root, const pipeline_cache_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, pipeline_cache_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_cache_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
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

