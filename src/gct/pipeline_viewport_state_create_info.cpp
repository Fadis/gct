#include <gct/shader_module.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <vulkan2json/PipelineViewportStateCreateInfo.hpp>
#include <vulkan2json/Viewport.hpp>
#include <vulkan2json/Rect2D.hpp>
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
#include <vulkan2json/PipelineViewportCoarseSampleOrderStateCreateInfoNV.hpp>
#include <vulkan2json/PipelineViewportShadingRateImageStateCreateInfoNV.hpp>
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
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( coarse_sample_order ) 
    LIBGCT_EXTENSION_TO_JSON( shading_rate_image ) 
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( exclusive_scissor ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( swizzle ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( w_scaling ) 
#endif
    root[ "viewport" ] = nlohmann::json::array();
    for( const auto &e: v.get_viewport() ) {
      root[ "viewport" ].push_back( e );
    }
    root[ "scissor" ] = nlohmann::json::array();
    for( const auto &e: v.get_scissor() ) {
      root[ "scissor" ].push_back( e );
    }
  }
  void from_json( const nlohmann::json &root, pipeline_viewport_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_viewport_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( coarse_sample_order ) 
    LIBGCT_EXTENSION_FROM_JSON( shading_rate_image ) 
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( exclusive_scissor ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( swizzle ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( w_scaling ) 
#endif
    if( !root[ "viewport" ].is_array() ) throw incompatible_json( "The JSON is incompatible to pipeline_viewport_state_create_info_t", __FILE__, __LINE__ );
    v.clear_viewport();
    for( const auto &e: root[ "viewport" ] ) {
      v.add_viewport( vk::Viewport( e ) );
    }
    if( !root[ "scissor" ].is_array() ) throw incompatible_json( "The JSON is incompatible to pipeline_viewport_state_create_info_t", __FILE__, __LINE__ );
    v.clear_scissor();
    for( const auto &e: root[ "scissor" ] ) {
      v.add_scissor( vk::Rect2D( e ) );
    }
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    if( !viewport.empty() ) {
      basic
        .setViewportCount( viewport.size() )
        .setPViewports( viewport.data() );
    }
    if( !scissor.empty() ) {
      basic
        .setScissorCount( scissor.size() )
        .setPScissors( scissor.data() );
    }
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_NV_SHADING_RATE_IMAGE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( coarse_sample_order ) 
    LIBGCT_EXTENSION_REBUILD_CHAIN( shading_rate_image ) 
#endif
#ifdef VK_NV_SCISSOR_EXCLUSIVE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( exclusive_scissor ) 
#endif
#ifdef VK_NV_VIEWPORT_SWIZZLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( swizzle ) 
#endif
#ifdef VK_NV_CLIP_SPACE_W_SCALING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( w_scaling ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::add_viewport( const vk::Viewport &v ) {
    viewport.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::clear_viewport() {
    viewport.clear();
    chained = false;
    return *this;
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::add_scissor( const vk::Rect2D &v ) {
    scissor.push_back( v );
    chained = false;
    return *this;
  }
  pipeline_viewport_state_create_info_t &pipeline_viewport_state_create_info_t::clear_scissor() {
    scissor.clear();
    chained = false;
    return *this;
  }
}

