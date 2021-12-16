#include <gct/device.hpp>
#include <gct/deferred_operation.hpp>

namespace gct {
  deferred_operation_t::deferred_operation_t(
    const std::shared_ptr< device_t > &device
  ) :
    created_from< device_t >( device ) {
    handle = (*device)->createDeferredOperationKHRUnique();
  }
}

