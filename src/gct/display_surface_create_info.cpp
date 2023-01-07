#include <gct/display_mode.hpp>
#include <gct/display_surface_create_info.hpp>
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
#include <vulkan2json/DisplaySurfaceCreateInfoKHR.hpp>
#endif

namespace gct {
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  void to_json( nlohmann::json &root, const display_surface_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
/*
 * This doesn't work due to lack of UniqueHandleTraits<DisplayModeKHR>
 */

#if 0
    if( v.get_display_mode() ) {
      root[ "basic" ][ "displayMode" ] = *v.get_display_mode();
    }
#endif
  }
  void from_json( const nlohmann::json &root, display_surface_create_info_t &v ) {
    if( !root.is_object() ) throw incompatible_json( "The JSON is incompatible to display_surface_create_info_t", __FILE__, __LINE__ );
    LIBGCT_EXTENSION_FROM_JSON( basic )
  }
  display_surface_create_info_t &display_surface_create_info_t::rebuild_chain() {
/*
 * This doesn't work due to lack of UniqueHandleTraits<DisplayModeKHR>
 */

#if 0
    if( chained ) return *this;
    if( display_mode )
      basic.setDisplayMode( **display_mode );
#endif
    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
/*
 * This doesn't work due to lack of UniqueHandleTraits<DisplayModeKHR>
 */

#if 0
  display_surface_create_info_t &display_surface_create_info_t::set_display_mode( const std::shared_ptr< display_mode_t > &mode ) {
    display_mode = mode;
    chained = false;
    return *this;
  }
  display_surface_create_info_t &display_surface_create_info_t::clear_display_mode() {
    display_mode.reset();
    chained = false;
    return *this;
  }
#endif
#endif
}

