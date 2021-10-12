#ifndef GCT_BUFFER_VIEW_CREATE_INFO_HPP
#define GCT_BUFFER_VIEW_CREATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class buffer_view_create_info_t {
  public:
    using self_type = buffer_view_create_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::BufferViewCreateInfo )
  };
}

#endif

