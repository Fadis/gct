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
    auto desc = props.get_descriptor_set_layout();
    for( auto &d: desc ) {
      if( !d ) {
        d = device->get_descriptor_set_layout();
      }
    }
    props.set_descriptor_set_layout( desc );
    props.rebuild_chain();
    handle = (*device)->createPipelineLayoutUnique( props.get_basic() );
  }
  void to_json( nlohmann::json &dest, const pipeline_layout_t &src ) {
    dest = nlohmann::json::object();
    dest[ "props" ] = src.get_props();
  }
}

