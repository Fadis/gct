#ifndef GCT_RAY_TRACING_PIPELINE_HPP
#define GCT_RAY_TRACING_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/ray_tracing_pipeline_create_info.hpp>
#include <gct/pipeline.hpp>
#include <gct/property.hpp>

namespace gct {
  class device_t;
  class deferred_operation_t;
  class ray_tracing_pipeline_t :
    public property< ray_tracing_pipeline_create_info_t >,
    public pipeline_t,
    public std::enable_shared_from_this< ray_tracing_pipeline_t > {
  public:
    ray_tracing_pipeline_t(
      const std::shared_ptr< pipeline_cache_t >&,
      const ray_tracing_pipeline_create_info_t&
    );
    ray_tracing_pipeline_t( const ray_tracing_pipeline_t& ) = delete;
    ray_tracing_pipeline_t( ray_tracing_pipeline_t&& ) = default;
    ray_tracing_pipeline_t &operator=( const ray_tracing_pipeline_t& ) = delete;
    ray_tracing_pipeline_t &operator=( ray_tracing_pipeline_t&& ) = default;
  };
  void to_json( nlohmann::json &root, const ray_tracing_pipeline_t &v );
}

#endif


