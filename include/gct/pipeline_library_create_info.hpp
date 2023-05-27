#ifndef GCT_PIPELINE_LIBRARY_CREATE_INFO_HPP
#define GCT_PIPELINE_LIBRARY_CREATE_INFO_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_PIPELINE_LIBRARY_EXTENSION_NAME
#include <memory>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_t;
  class pipeline_library_create_info_t : public chained_t {
  public:
    using self_type = pipeline_library_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineLibraryCreateInfoKHR )
  private:
    std::vector< std::shared_ptr< pipeline_t > > library;
    std::vector< vk::Pipeline > raw_library;
  public:
    pipeline_library_create_info_t &add_library( const std::shared_ptr< pipeline_t >& );
    pipeline_library_create_info_t &clear_library();
    const std::vector< std::shared_ptr< pipeline_t > > &get_library() const { return library; }
  };
  void to_json( nlohmann::json &root, const pipeline_library_create_info_t &v );
}
#endif

#endif

