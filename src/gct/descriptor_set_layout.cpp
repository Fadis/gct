#include <gct/device.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <nlohmann/json.hpp>

namespace gct {
  descriptor_set_layout_t::descriptor_set_layout_t(
    const std::shared_ptr< device_t > &device,
    const descriptor_set_layout_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createDescriptorSetLayoutUnique(
      props.get_basic()
    );
  }
  void to_json( nlohmann::json &root, const descriptor_set_layout_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}

