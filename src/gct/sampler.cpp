#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/sampler.hpp>

namespace gct {
  sampler_t::sampler_t(
    const std::shared_ptr< device_t > &device,
    const sampler_create_info_t &create_info
  ) :
    created_from< device_t >( device ),
    props( create_info ) {
    props.rebuild_chain();
    handle = (*device)->createSamplerUnique( props.get_basic() );
  }
  void to_json( nlohmann::json &root, const sampler_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}

