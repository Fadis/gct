#ifndef GCT_IMAGE_VIEW_HPP
#define GCT_IMAGE_VIEW_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/image_view_create_info.hpp>

namespace gct {
  struct allocator_t;
  class image_t;
  class image_view_t : public created_from< image_t > {
  public:
    image_view_t(
      const std::shared_ptr< image_t >&,
      const image_view_create_info_t&
    );
    image_view_t( const image_view_t& ) = delete;
    image_view_t( image_view_t&& ) = default;
    image_view_t &operator=( const image_view_t& ) = delete;
    image_view_t &operator=( image_view_t&& ) = default;
    const image_view_create_info_t &get_props() const { return props; }
    vk::ImageView &operator*() {
      return *handle;
    }
    const vk::ImageView &operator*() const {
      return *handle;
    }
    vk::ImageView *operator->() {
      return &handle.get();
    }
    const vk::ImageView *operator->() const {
      return &handle.get();
    }
  private:
    image_view_create_info_t props;
    vk::UniqueHandle< vk::ImageView, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > handle;
  };
}

#endif

