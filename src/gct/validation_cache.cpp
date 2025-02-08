#include <vulkan/vulkan.hpp>
#ifdef VK_EXT_VALIDATION_CACHE_EXTENSION_NAME
#include <gct/device.hpp>
#include <gct/exception.hpp>
#include <gct/validation_cache.hpp>
#include <nlohmann/json.hpp>

namespace gct {
  validation_cache_t::validation_cache_t(
    const std::shared_ptr< device_t > &device,
    const validation_cache_create_info_t &create_info
  ) :
    property_type( create_info ),
    created_from< device_t >( device ) {
    props.rebuild_chain();
    handle = (*device)->createValidationCacheEXTUnique( props.get_basic() );
  }
  void to_json( nlohmann::json &root, const validation_cache_t &v ) {
    root = nlohmann::json::object();
    root[ "props" ] = v.get_props();
  }
}
#endif

