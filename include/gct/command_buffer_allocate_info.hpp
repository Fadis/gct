#ifndef GCT_COMMAND_BUFFER_ALLOCATE_INFO_HPP
#define GCT_COMMAND_BUFFER_ALLOCATE_INFO_HPP
#include <memory>
#include <optional>
#include <vulkan/vulkan.hpp>
#include <gct/extension.hpp>

namespace gct {
  class command_buffer_allocate_info_t : public chained_t {
  public:
    using self_type = command_buffer_allocate_info_t;
    LIBGCT_EXTENSION_REBUILD_CHAIN_DEF
    LIBGCT_BASIC_SETTER( vk::CommandBufferAllocateInfo )
  };
}

#endif

