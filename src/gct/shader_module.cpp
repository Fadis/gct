#include <gct/device.hpp>
#include <gct/shader_module.hpp>

namespace gct {
  shader_module_t::shader_module_t(
    const std::shared_ptr< device_t > &device,
    const shader_module_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createShaderModuleUnique( props.get_basic() );
  }
}
