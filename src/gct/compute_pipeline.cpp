#include <gct/device.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/compute_pipeline.hpp>

namespace gct {
  compute_pipeline_t::compute_pipeline_t(
    const std::shared_ptr< pipeline_cache_t > &cache,
    const compute_pipeline_create_info_t &create_info
  ) :
    created_from< pipeline_cache_t >( cache ),
    props( create_info ) {
    props.rebuild_chain();

    auto wrapped = (*cache->get_factory())->createComputePipelinesUnique( **cache, { props.get_basic() } );
    if( wrapped.result != vk::Result::eSuccess )
      vk::throwResultException( wrapped.result, "createComputePipeline failed" );
    handle = std::move( wrapped.value[ 0 ] );
  }
}
