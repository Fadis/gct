#ifndef GCT_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_HPP
#define GCT_RAY_TRACING_PIPELINE_INTERFACE_CREATE_INFO_HPP
#include <vulkan/vulkan.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <memory>
#include <optional>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
  class ray_tracing_pipeline_interface_create_info_t : public chained_t {
  public:
    using self_type = ray_tracing_pipeline_interface_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::RayTracingPipelineInterfaceCreateInfoKHR )
  };
  void to_json( nlohmann::json &root, const ray_tracing_pipeline_interface_create_info_t &v );
  void from_json( const nlohmann::json &root, ray_tracing_pipeline_interface_create_info_t &v );
}

#endif

#endif
