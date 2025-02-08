#ifndef GCT_VULKAN_HANDLE_HPP
#define GCT_VULKAN_HANDLE_HPP

#include <vulkan/vulkan.hpp>

namespace gct {
  template< typename T >
  class vulkan_handle {
  public:
    using handle_type = vulkan_handle< T >;
    using raw_handle_type = T;
    vulkan_handle() = default;
    vulkan_handle( const vulkan_handle& ) = delete;
    vulkan_handle( vulkan_handle&& ) = default;
    vulkan_handle &operator=( const vulkan_handle& ) = delete;
    vulkan_handle &operator=( vulkan_handle&& ) = default;
    [[nodiscard]] raw_handle_type &operator*() {
      return *handle;
    }
    [[nodiscard]] const raw_handle_type &operator*() const {
      return *handle;
    }
    [[nodiscard]] raw_handle_type *operator->() {
      return &handle.get();
    }
    [[nodiscard]] const raw_handle_type *operator->() const {
      return &handle.get();
    }
  protected:
// NOLINTBEGIN(cppcoreguidelines-non-private-member-variables-in-classes)
    vk::UniqueHandle< raw_handle_type, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
// NOLINTEND(cppcoreguidelines-non-private-member-variables-in-classes)
  };
}

#endif

