#include <gct/ray_tracing_pipeline_create_info.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/RayTracingPipelineCreateInfoKHR.hpp>
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PipelineCreationFeedbackCreateInfoEXT.hpp>
#endif
#include <gct/extension.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const ray_tracing_pipeline_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( creation_feedback )
#endif
  }
  void from_json( const nlohmann::json &root, ray_tracing_pipeline_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to ray_tracing_pipeline_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_FROM_JSON( creation_feedback )
#endif
  }
  ray_tracing_pipeline_create_info_t &ray_tracing_pipeline_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_feedback )
#endif
  }
}
#endif

