#include <gct/device.hpp>
#include <gct/descriptor_set_layout.hpp>

namespace gct {
  descriptor_set_layout_t::descriptor_set_layout_t(
    const std::shared_ptr< device_t > &device,
    const descriptor_set_layout_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createDescriptorSetLayoutUnique(
      props.get_basic()
    );
  }
}

