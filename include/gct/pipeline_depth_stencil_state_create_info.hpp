#ifndef GCT_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_depth_stencil_state_create_info_t {
  public:
    using self_type = pipeline_depth_stencil_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineDepthStencilStateCreateInfo )
  };
  void to_json( nlohmann::json &root, const pipeline_depth_stencil_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_depth_stencil_state_create_info_t &v );
}

#endif

