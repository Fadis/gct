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
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN_WRAPPED( basic, LibraryCount, PLibraries, library )
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

