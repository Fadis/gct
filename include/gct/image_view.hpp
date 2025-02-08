#ifndef GCT_IMAGE_VIEW_HPP
#define GCT_IMAGE_VIEW_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/created_from.hpp>
#include <gct/image_view_create_info.hpp>
#include <gct/vulkan_handle.hpp>
#include <gct/property.hpp>

namespace gct {
  struct allocator_t;
  class image_t;
  class image_view_t :
    public vulkan_handle< vk::ImageView >,
    public property< image_view_create_info_t >,
    public created_from< image_t > {
  public:
    image_view_t(
      const std::shared_ptr< image_t >&,
      const image_view_create_info_t&
    );
    image_view_t( const image_view_t& ) = delete;
    image_view_t( image_view_t&& ) = default;
    image_view_t &operator=( const image_view_t& ) = delete;
    image_view_t &operator=( image_view_t&& ) = default;
  };
  void to_json( nlohmann::json&, const image_view_t& );
}

#endif

