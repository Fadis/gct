#ifndef GCT_BUFFER_VIEW_HPP
#define GCT_BUFFER_VIEW_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/created_from.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct allocator_t;
  class buffer_t;
  class buffer_view_t :
    public vulkan_handle< vk::BufferView >,
    public property< buffer_view_create_info_t >,
    public created_from< buffer_t > {
  public:
    buffer_view_t(
      const std::shared_ptr< buffer_t >&,
      const buffer_view_create_info_t&
    );
    buffer_view_t( const buffer_view_t& ) = delete;
    buffer_view_t( buffer_view_t&& ) = default;
    buffer_view_t &operator=( const buffer_view_t& ) = delete;
    buffer_view_t &operator=( buffer_view_t&& ) = default;
  };
}

#endif

