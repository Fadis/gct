#include <gct/device.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/graphics_pipeline.hpp>

namespace gct {
  graphics_pipeline_t::graphics_pipeline_t(
    const std::shared_ptr< pipeline_cache_t > &cache,
    const graphics_pipeline_create_info_t &create_info
  ) :
    created_from< pipeline_cache_t >( cache ),
    props( create_info ) {
    props.rebuild_chain();
    if( props.get_basic().pDepthStencilState )
      std::cout << "depth_stencil" << " " << props.get_basic().pDepthStencilState->depthTestEnable << std::endl;
    else
      std::cout << "no depth_stencil" << " " << std::endl;

    auto wrapped = (*cache->get_factory())->createGraphicsPipelinesUnique( **cache, { props.get_basic() } );
    if( wrapped.result != vk::Result::eSuccess )
      vk::throwResultException( wrapped.result, "createGraphicsPipeline failed" );
    handle = std::move( wrapped.value[ 0 ] );
  }
}
