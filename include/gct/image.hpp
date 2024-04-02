#ifndef GCT_IMAGE_HPP
#define GCT_IMAGE_HPP
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image_layout.hpp>

namespace gct {
  vk::ImageViewType to_image_view_type( vk::ImageType v, std::uint32_t );
  class image_view_create_info_t;
  class device_t;
  class image_t {
  public:
    image_t(
      const image_create_info_t&
    );
    virtual ~image_t() {}
    const image_create_info_t &get_props() const { return props; }
    virtual std::shared_ptr< image_view_t > get_view(
      const image_view_create_info_t&
    ) = 0;
    virtual std::shared_ptr< image_view_t > get_view(
      vk::ImageAspectFlags aspect
    ) = 0;
    virtual std::shared_ptr< image_view_t > get_view() = 0;
    virtual vk::Image &operator*() = 0;
    virtual const vk::Image &operator*() const = 0;
    virtual vk::Image *operator->() = 0;
    virtual const vk::Image *operator->() const = 0;
    virtual std::shared_ptr< device_t >get_device() const = 0;
    const image_layout_t &get_layout() const {
      return layout;
    };
    image_layout_t &get_layout() {
      return layout;
    };
  protected:
    image_create_info_t props;
    image_layout_t layout;
  };

  std::vector< std::shared_ptr< image_view_t > >
  get_views(
    const std::vector< std::shared_ptr< image_t > > &
  );

void to_json( nlohmann::json&, const image_t& );

}

#endif

