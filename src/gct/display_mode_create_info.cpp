#include <gct/display_mode_create_info.hpp>
#include <vulkan2json/DisplayModeCreateInfoKHR.hpp>

namespace gct {
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  void to_json( nlohmann::json &root, const display_mode_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
  }
  void from_json( const nlohmann::json &root, display_mode_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to display_mode_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  display_mode_create_info_t &display_mode_create_info_t::rebuild_chain() {
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
#endif
}

