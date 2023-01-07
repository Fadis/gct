#ifndef GCT_DISPLAY_SURFACE_CREATE_INFO_HPP
#define GCT_DISPLAY_SURFACE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
namespace gct {
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  class display_mode_t;
  class display_surface_create_info_t : public chained_t {
  public:
    using self_type = display_surface_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DisplaySurfaceCreateInfoKHR )
/*
 * This doesn't work due to lack of UniqueHandleTraits<DisplayModeKHR>
 */

#if 0
    display_surface_create_info_t &set_display_mode( const std::shared_ptr< display_mode_t >& );
    display_surface_create_info_t &clear_display_mode();
    const std::shared_ptr< display_mode_t > &get_display_mode() { return display_mode; }
  private:
    std::shared_ptr< display_mode_t > display_mode;
#endif
  };
  void to_json( nlohmann::json &root, const display_surface_create_info_t &v );
  void from_json( const nlohmann::json &root, display_surface_create_info_t &v );
#endif
}

#endif


