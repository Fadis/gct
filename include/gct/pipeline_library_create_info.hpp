#ifndef GCT_PIPELINE_LIBRARY_CREATE_INFO_HPP
#define GCT_PIPELINE_LIBRARY_CREATE_INFO_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_library_create_info_t : public chained_t {
  public:
    using self_type = pipeline_library_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineLibraryCreateInfoKHR )
    //////
  };
  void to_json( nlohmann::json &root, const pipeline_library_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_library_create_info_t &v );
}
#endif

#endif

