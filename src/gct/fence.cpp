#include <gct/device.hpp>
#include <gct/fence.hpp>

namespace gct {
  fence_t::fence_t(
    const std::shared_ptr< device_t > &device,
    const fence_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createFenceUnique( props.get_basic() );
  }
}
