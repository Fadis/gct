#ifndef GCT_BUFFER_VIEW_CREATE_INFO_HPP
#define GCT_BUFFER_VIEW_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/extension.hpp>
namespace gct {
  class buffer_view_create_info_t {
  public:
    using self_type = buffer_view_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::BufferViewCreateInfo )
  };
  void to_json( nlohmann::json &root, const buffer_view_create_info_t &v );
  void from_json( const nlohmann::json &root, buffer_view_create_info_t &v );
}

#endif

