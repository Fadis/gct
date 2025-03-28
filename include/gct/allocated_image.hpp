#ifndef GCT_ALLOCATED_IMAGE_HPP
#define GCT_ALLOCATED_IMAGE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wtype-limits"
#pragma GCC diagnostic ignored "-Wmissing-field-initializers"
#pragma GCC diagnostic ignored "-Wunused-variable"
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wreorder"
#pragma GCC diagnostic ignored "-Wclass-memaccess"
#include <gct/vk_mem_alloc.h>
#pragma GCC diagnostic pop
#include <gct/created_from.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>

namespace gct {
  class allocated_image_t :
    public image_t,
    public created_from< allocator_t >,
    public std::enable_shared_from_this< allocated_image_t > {
  public:
    allocated_image_t(
      const std::shared_ptr< allocator_t >&,
      const image_create_info_t&,
      VmaMemoryUsage usage
    );
    allocated_image_t( const allocated_image_t& ) = delete;
    allocated_image_t( allocated_image_t&& ) = default;
    allocated_image_t &operator=( const allocated_image_t& ) = delete;
    allocated_image_t &operator=( allocated_image_t&& ) = default;
//    [[nodiscard]] const image_create_info_t &get_props() const { return props; }
    [[nodiscard]] std::shared_ptr< image_view_t > get_view(
      const image_view_create_info_t&
    );
    [[nodiscard]] std::shared_ptr< image_view_t > get_view(
      vk::ImageAspectFlags aspect,
      bool force_array = false
    );
    [[nodiscard]] std::vector< std::shared_ptr< image_view_t > > get_thin_views(
      vk::ImageAspectFlags aspect,
      std::uint32_t layer,
      bool force_array = false
    );
    [[nodiscard]] std::shared_ptr< image_view_t > get_view(
      bool force_array = false
    );
    [[nodiscard]] std::vector< std::shared_ptr< image_view_t > > get_thin_views(
      std::uint32_t layer,
      bool force_array = false
    );
    [[nodiscard]] virtual std::shared_ptr< image_view_t > get_view(
      const subview_range &
    );
    [[nodiscard]] vk::Image &operator*() {
      return *handle;
    }
    [[nodiscard]] const vk::Image &operator*() const {
      return *handle;
    }
    [[nodiscard]] vk::Image *operator->() {
      return handle.get();
    }
    [[nodiscard]] const vk::Image *operator->() const {
      return handle.get();
    }
    [[nodiscard]] std::shared_ptr< device_t > get_device() const;
  private:
    std::shared_ptr< vk::Image > handle;
    std::shared_ptr< VmaAllocation > allocation;
  };
}
#endif

