#ifndef GCT_PIPELINE_TESSELLATION_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_TESSELLATION_STATE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_tessellation_state_create_info_t {
  public:
    using self_type = pipeline_tessellation_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineTessellationStateCreateInfo )
#ifdef VK_VERSION_1_1
    LIBGCT_EXTENSION_SETTER( vk::PipelineTessellationDomainOriginStateCreateInfo, domain_origin )
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_SETTER( vk::PipelineTessellationDomainOriginStateCreateInfoKHR , domain_origin )
#endif
  };
  void to_json( nlohmann::json &root, const pipeline_tessellation_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_tessellation_state_create_info_t &v );
}

#endif

