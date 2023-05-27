#include <nlohmann/json.hpp>
#include <gct/shader_module.hpp>
#include <gct/pipeline_rasterization_state_create_info.hpp>
#include <vulkan2json/PipelineRasterizationStateCreateInfo.hpp>
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
#include <vulkan2json/PipelineRasterizationConservativeStateCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
#include <vulkan2json/PipelineRasterizationDepthClipStateCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
#include <vulkan2json/PipelineRasterizationLineStateCreateInfoEXT.hpp>
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
#include <vulkan2json/PipelineRasterizationProvokingVertexStateCreateInfoEXT.hpp>
#endif
#ifdef VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
#include <vulkan2json/PipelineRasterizationStateRasterizationOrderAMD.hpp>
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PipelineRasterizationStateStreamCreateInfoEXT.hpp>
#endif

namespace gct {
  void to_json( nlohmann::json &root, const pipeline_rasterization_state_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( conservative )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( depth_clip )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( line )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( provoking_vertex )
#endif
#ifdef VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( rasterization_order )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( stream )
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_rasterization_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_rasterization_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( conservative )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( depth_clip )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( line )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( provoking_vertex )
#endif
#ifdef VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( rasterization_order )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( stream )
#endif
  }
  pipeline_rasterization_state_create_info_t &pipeline_rasterization_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_EXT_CONSERVATIVE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( conservative )
#endif
#ifdef VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( depth_clip )
#endif
#ifdef VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( line )
#endif
#ifdef VK_EXT_PROVOKING_VERTEX_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( provoking_vertex )
#endif
#ifdef VK_AMD_RASTERIZATION_ORDER_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( rasterization_order )
#endif
#ifdef VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( stream )
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

