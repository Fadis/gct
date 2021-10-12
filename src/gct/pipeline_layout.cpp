#include <gct/device.hpp>
#include <gct/pipeline_layout.hpp>

namespace gct {
  pipeline_layout_t::pipeline_layout_t(
    const std::shared_ptr< device_t > &device,
    const pipeline_layout_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createPipelineLayoutUnique( props.get_basic() );
  }
}
