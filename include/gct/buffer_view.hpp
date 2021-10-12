#ifndef GCT_BUFFER_VIEW_HPP
#define GCT_BUFFER_VIEW_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/buffer_view_create_info.hpp>

namespace gct {
  struct allocator_t;
  class buffer_t;
  class buffer_view_t : public created_from< buffer_t > {
  public:
    buffer_view_t(
      const std::shared_ptr< buffer_t >&,
      const buffer_view_create_info_t&
    );
    buffer_view_t( const buffer_view_t& ) = delete;
    buffer_view_t( buffer_view_t&& ) = default;
    buffer_view_t &operator=( const buffer_view_t& ) = delete;
    buffer_view_t &operator=( buffer_view_t&& ) = default;
    const buffer_view_create_info_t &get_props() const { return props; }
    vk::BufferView &operator*() {
      return *handle;
    }
    const vk::BufferView &operator*() const {
      return *handle;
    }
    vk::BufferView *operator->() {
      return &handle.get();
    }
    const vk::BufferView *operator->() const {
      return &handle.get();
    }
  private:
    buffer_view_create_info_t props;
    vk::UniqueHandle< vk::BufferView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

