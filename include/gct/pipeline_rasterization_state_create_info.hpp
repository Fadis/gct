#ifndef GCT_PIPELINE_RASTERIZATION_STATE_CREATE_INFO_HPP
#define GCT_PIPELINE_RASTERIZATION_STATE_CREATE_INFO_HPP
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class pipeline_rasterization_state_create_info_t : public chained_t {
  public:
    using self_type = pipeline_rasterization_state_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::PipelineRasterizationStateCreateInfo )
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRasterizationConservativeStateCreateInfoEXT, conservative )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRasterizationDepthClipStateCreateInfoEXT, depth_clip )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRasterizationLineStateCreateInfoEXT, line )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRasterizationProvokingVertexStateCreateInfoEXT, provoking_vertex )
#endif
#ifdef VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRasterizationStateRasterizationOrderAMD, rasterization_order )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_SETTER( vk::PipelineRasterizationStateStreamCreateInfoEXT, stream )
#endif
    pipeline_rasterization_state_create_info_t();
  };
  void to_json( nlohmann::json &root, const pipeline_rasterization_state_create_info_t &v );
  void from_json( const nlohmann::json &root, pipeline_rasterization_state_create_info_t &v );
}

#endif

