#include <gct/device.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/ray_tracing_pipeline.hpp>
#include <gct/deferred_operation.hpp>

namespace gct {
  ray_tracing_pipeline_t::ray_tracing_pipeline_t(
    const std::shared_ptr< pipeline_cache_t > &cache,
    const std::shared_ptr< deferred_operation_t > &deferred_operation_,
    const ray_tracing_pipeline_create_info_t &create_info
  ) :
    pipeline_t( cache ),
    deferred_operation( deferred_operation_ ),
    props( create_info ) {
    props.rebuild_chain();

    auto wrapped = (*cache->get_factory())->createRayTracingPipelinesKHRUnique( **deferred_operation, **cache, { props.get_basic() } );
    if( wrapped.result != vk::Result::eSuccess )
      vk::throwResultException( wrapped.result, "createRayTracingPipeline failed" );
    handle = std::move( wrapped.value[ 0 ] );
  }
}
