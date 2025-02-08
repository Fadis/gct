#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_layout.hpp>

namespace gct {
  pipeline_layout_t::pipeline_layout_t(
    const std::shared_ptr< device_t > &device,
    const pipeline_layout_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createPipelineLayoutUnique( props.get_basic() );
  }
}
