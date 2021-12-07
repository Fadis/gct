#include <gct/shader_module.hpp>
#include <gct/pipeline_tessellation_state_create_info.hpp>
#include <vulkan2json/PipelineTessellationStateCreateInfo.hpp>
#ifdef VK_VERSION_1_1
#include <vulkan2json/PipelineTessellationDomainOriginStateCreateInfo.hpp>
#elif defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
#include <vulkan2json/PipelineTessellationDomainOriginStateCreateInfoKHR.hpp>
#endif
namespace gct {
  void to_json( nlohmann::json &root, const pipeline_tessellation_state_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( domain_origin ) 
#endif
  }
  void from_json( const nlohmann::json &root, pipeline_tessellation_state_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to pipeline_tessellation_state_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_FROM_JSON( domain_origin ) 
#endif
  }
  pipeline_tessellation_state_create_info_t &pipeline_tessellation_state_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#if defined(VK_VERSION_1_1) || defined(VK_KHR_MAINTENANCE2_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( domain_origin ) 
#endif
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}

