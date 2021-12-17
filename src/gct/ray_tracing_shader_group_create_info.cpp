#include <gct/ray_tracing_shader_group_create_info.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <vulkan2json/RayTracingShaderGroupCreateInfoKHR.hpp>
#include <gct/extension.hpp>
namespace gct {
  void to_json( nlohmann::json &root, const ray_tracing_shader_group_create_info_t &v ) {
     root = nlohmann::json::object();
     root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, ray_tracing_shader_group_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to ray_tracing_shader_group_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  ray_tracing_shader_group_create_info_t &ray_tracing_shader_group_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
}
#endif

