#include <gct/device.hpp>
#include <gct/semaphore.hpp>

namespace gct {
  semaphore_t::semaphore_t(
    const std::shared_ptr< device_t > &device,
    const semaphore_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createSemaphoreUnique( props.get_basic() );
  }
}
