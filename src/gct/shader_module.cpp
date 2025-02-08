#include <gct/device.hpp>
#include <gct/exception.hpp>
#include <gct/shader_module.hpp>
#include <nlohmann/json.hpp>

namespace gct {
  shader_module_t::shader_module_t(
    const std::shared_ptr< device_t > &device,
    const shader_module_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createShaderModuleUnique( props.get_basic() );
  }
  void to_json( nlohmann::json &root, const shader_module_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}
