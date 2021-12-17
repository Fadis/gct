#include <gct/device.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/compute_pipeline.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <gct/ray_tracing_pipeline.hpp>
#endif
#include <gct/pipeline_cache.hpp>

namespace gct {
  pipeline_cache_t::pipeline_cache_t(
    const std::shared_ptr< device_t > &device,
    const pipeline_cache_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createPipelineCacheUnique(
      props.get_basic()
    );
  }
  std::shared_ptr< compute_pipeline_t > pipeline_cache_t::get_pipeline(
    const compute_pipeline_create_info_t &create_info
  ) {
    return std::shared_ptr< compute_pipeline_t >(
      new compute_pipeline_t(
        shared_from_this(),
        create_info
      )
    );
  }
  std::shared_ptr< graphics_pipeline_t > pipeline_cache_t::get_pipeline(
    const graphics_pipeline_create_info_t &create_info
  ) {
    return std::shared_ptr< graphics_pipeline_t >(
      new graphics_pipeline_t(
        shared_from_this(),
        create_info
      )
    );
  }
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  std::shared_ptr< ray_tracing_pipeline_t > pipeline_cache_t::get_pipeline(
    const std::shared_ptr< deferred_operation_t > &deferred_operation,
    const ray_tracing_pipeline_create_info_t &create_info
  ) {
    return std::shared_ptr< ray_tracing_pipeline_t >(
      new ray_tracing_pipeline_t(
        shared_from_this(),
        deferred_operation,
        create_info
      )
    );
  }
#endif
}
