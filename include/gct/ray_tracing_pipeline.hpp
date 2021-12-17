#ifndef GCT_RAY_TRACING_PIPELINE_HPP
#define GCT_RAY_TRACING_PIPELINE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/ray_tracing_pipeline_create_info.hpp>
#include <gct/pipeline.hpp>

namespace gct {
  class device_t;
  class deferred_operation_t;
  class ray_tracing_pipeline_t : public pipeline_t, public std::enable_shared_from_this< ray_tracing_pipeline_t > {
  public:
    ray_tracing_pipeline_t(
      const std::shared_ptr< pipeline_cache_t >&,
      const std::shared_ptr< deferred_operation_t >&,
      const ray_tracing_pipeline_create_info_t&
    );
    ray_tracing_pipeline_t( const ray_tracing_pipeline_t& ) = delete;
    ray_tracing_pipeline_t( ray_tracing_pipeline_t&& ) = default;
    ray_tracing_pipeline_t &operator=( const ray_tracing_pipeline_t& ) = delete;
    ray_tracing_pipeline_t &operator=( ray_tracing_pipeline_t&& ) = default;
    const ray_tracing_pipeline_create_info_t &get_props() const { return props; }
  private:
    std::shared_ptr< deferred_operation_t > deferred_operation;
    ray_tracing_pipeline_create_info_t props;
  };
}

#endif


