#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <vulkan2json/PipelineViewportStateCreateInfo.hpp>
#include <vulkan2json/Viewport.hpp>
#include <vulkan2json/Rect2D.hpp>
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
#include <vulkan2json/PipelineViewportCoarseSampleOrderStateCreateInfoNV.hpp>
#include <vulkan2json/PipelineViewportShadingRateImageStateCreateInfoNV.hpp>
#include <vulkan2json/CoarseSampleOrderCustomNV.hpp>
#include <vulkan2json/ShadingRatePaletteNV.hpp>
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
#include <vulkan2json/PipelineViewportDepthClipControlCreateInfoEXT.hpp>
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
#include <vulkan2json/PipelineViewportExclusiveScissorStateCreateInfoNV.hpp>
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
#include <vulkan2json/PipelineViewportSwizzleStateCreateInfoNV.hpp>
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
#include <vulkan2json/PipelineViewportWScalingStateCreateInfoNV.hpp>
#endif
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_viewport_state_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    LIBGCT_ARRAY_OF_TO_JSON( basic, pViewports, viewport )
    LIBGCT_ARRAY_OF_TO_JSON( basic, pScissors, scissor )
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coarse_sample_order_state ) 
    LIBGCT_EXTENSION_TO_JSON( shading_rate_image_state ) 
    LIBGCT_ARRAY_OF_TO_JSON( coarse_sample_order_state, pCustomSampleOrders, coarse_sample_order )
    LIBGCT_ARRAY_OF_TO_JSON( shading_rate_image_state, pShadingRatePalettes, shading_rate_palette )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_clip_control )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( exclusive_scissor_state ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( swizzle_state ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( w_scaling_state ) 
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_viewport_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_viewport_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pViewports, viewport )
    LIBGCT_ARRAY_OF_FROM_JSON( basic, pScissors, scissor )
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( coarse_sample_order_state ) 
    LIBGCT_EXTENSION_FROM_JSON( shading_rate_image_state ) 
    LIBGCT_ARRAY_OF_FROM_JSON( coarse_sample_order_state, pCustomSampleOrders, coarse_sample_order )
    LIBGCT_ARRAY_OF_FROM_JSON( shading_rate_image_state, pShadingRatePalettes, shading_rate_palette )
#endif
#ifdef VK_EXT_DEPTH_CLIP_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( depth_clip_control )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( exclusive_scissor_state ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( swizzle_state ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( w_scaling_state ) 
#endif
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, ViewportCount, PViewports, viewport )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( basic, ScissorCount, PScissors, scissor )
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coarse_sample_order_state ) 
    LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image_state )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( coarse_sample_order_state, CustomSampleOrderCount, PCustomSampleOrders, coarse_sample_order )
    LIBGCT_ARRAY_OF_REBUILD_CHAIN( shading_rate_image_state, ViewportCount, PShadingRatePalettes, shading_rate_palette )
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( exclusive_scissor_state ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( swizzle_state ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( w_scaling_state ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::add_size(
    unsigned int width,
    unsigned int height
  ) {
    add_viewport(
      vk::Viewport()
        .setWidth( width )
        .setHeight( height )
        .setMinDepth( 0.0f )
        .setMaxDepth( 1.0f )
    );
    add_scissor(
      vk::Rect2D()
        .setOffset( { 0, 0 } )
        .setExtent( { width, height } )
    );
    chained = false;
    return *this;
  }
}

