#ifndef GCT_PIPELINE_VIEWPORT_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_VIEWPORT_STATE_CREATE_INFO_HPP
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>
#include <gct/array_of.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
  class pipeline_viewport_state_create_info_t : public chained_t {
  public:
    using self_type = pipeline_viewport_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineViewportStateCreateInfo )
    LIBGCT_ARRAY_OF( vk::Viewport, viewport )
    LIBGCT_ARRAY_OF( vk::Rect2D, scissor )
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportCoarseSampleOrderStateCreateInfoNV, coarse_sample_order_state )
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportShadingRateImageStateCreateInfoNV, shading_rate_image_state )
    LIBGCT_ARRAY_OF( vk::CoarseSampleOrderCustomNV, coarse_sample_order )
    LIBGCT_ARRAY_OF( vk::ShadingRatePaletteNV, shading_rate_palette )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportDepthClipControlCreateInfoEXT , depth_clip_control )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportExclusiveScissorStateCreateInfoNV , exclusive_scissor_state )
    LIBGCT_ARRAY_OF( vk::Rect2D, exclusive_sissor )
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportSwizzleStateCreateInfoNV, swizzle_state )
    LIBGCT_ARRAY_OF( vk::ViewportSwizzleNV, swizzle )
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineViewportWScalingStateCreateInfoNV, w_scaling_state )
    LIBGCT_ARRAY_OF( vk::ViewportWScalingNV, w_scaling )
#endif
    pipeline_viewport_state_create_info_t &add_size(
      unsigned int width,
      unsigned int height
    );
  };
  void to_json( nlohmann::json &root, const pipeline_viewport_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_viewport_state_create_info_t &v );
}

#endif

