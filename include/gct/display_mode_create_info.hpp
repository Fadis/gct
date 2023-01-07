#ifndef GCT_DISPLAY_MODE_CREATE_INFO_HPP
#define GCT_DISPLAY_MODE_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>

namespace gct {
#ifdef VK_KHR_DISPLAY_EXTENSION_NAME
  class display_mode_create_info_t : public chained_t {
  public:
    using self_type = display_mode_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::DisplayModeCreateInfoKHR )
  };
  void to_json( nlohmann::json &root, const display_mode_create_info_t &v );
  void from_json( const nlohmann::json &root, display_mode_create_info_t &v );
#endif
}

#endif


