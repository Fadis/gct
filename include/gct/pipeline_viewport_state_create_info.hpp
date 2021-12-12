#ifndef GCT_PIPELINE_VIEWPORT_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_VIEWPORT_STATE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_viewport_state_create_info_t : public chained_t {
  public:
    using self_type = pipeline_viewport_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineViewportStateCreateInfo )
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportCoarseSampleOrderStateCreateInfoNV, coarse_sample_order )
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportShadingRateImageStateCreateInfoNV, shading_rate_image )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportExclusiveScissorStateCreateInfoNV , exclusive_scissor )
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
     LIBGCT_EXTENSION_SETTER( vk::PipelineViewportSwizzleStateCreateInfoNV, swizzle )
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
     LIBGCT_EXTENSION_SETTER( vk::PipelineViewportWScalingStateCreateInfoNV, w_scaling )
#endif
  };
  void to_json( nlohmann::json &root, const pipeline_viewport_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_viewport_state_create_info_t &v );
}

#endif

