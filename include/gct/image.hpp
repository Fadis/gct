#ifndef GCT_IMAGE_HPP
#define GCT_IMAGE_HPP
#include <memory>
#include <vulkan/vulkan.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>

namespace gct {
  vk::ImageViewType to_image_view_type( vk::ImageType v );
  class image_view_create_info_t;
  class device_t;
  class image_t {
  public:
    image_t(
      const image_create_info_t&
    );
    virtual ~image_t() {}
    const image_create_info_t &get_props() const { return props; }
    void set_layout( vk::ImageLayout l ) {
      auto basic = props.get_basic();
      basic.initialLayout = l;
      props.set_basic( basic ).rebuild_chain();
    }
    virtual std::shared_ptr< image_view_t > get_view(
      const image_view_create_info_t&
    ) = 0;
    virtual std::shared_ptr< image_view_t > get_view(
      vk::ImageAspectFlags aspect
    ) = 0;
    virtual vk::Image &operator*() = 0;
    virtual const vk::Image &operator*() const = 0;
    virtual vk::Image *operator->() = 0;
    virtual const vk::Image *operator->() const = 0;
    virtual std::shared_ptr< device_t >get_device() const = 0;
  protected:
    image_create_info_t props;
  };
}

#endif

