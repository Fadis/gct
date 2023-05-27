#include <nlohmann/json.hpp>
#include <gct/pipeline_library_create_info.hpp>
#include <gct/pipeline.hpp>
#include <vulkan2json/PipelineLibraryCreateInfoKHR.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_library_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
  }
  pipeline_library_create_info_t &pipeline_library_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    
    raw_library.clear();
    raw_library.reserve( library.size() );
    std::transform(
      library.begin(),
      library.end(),
      std::back_inserter( raw_library ),
      []( const auto &v ) {
        return **v;
      }
    );
    if( !raw_library.empty() )
      basic
        .setLibraryCount( raw_library.size() )
        .setPLibraries( raw_library.data() );
    else
      basic
        .setLibraryCount( 0u )
        .setPLibraries( nullptr );

    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_library_create_info_t &pipeline_library_create_info_t::add_library( const std::shared_ptr< pipeline_t > &v ) {
    library.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_library_create_info_t &pipeline_library_create_info_t::clear_library() {
    library.clear();
    chained = false;
    return *this;
  }
}

