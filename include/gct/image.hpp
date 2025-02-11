#ifndef GCT_IMAGE_HPP
#define GCT_IMAGE_HPP
#include <cstdint>
#include <memory>
#include <nlohmann/json_fwd.hpp>
#include <vulkan/vulkan.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image_view.hpp>
#include <gct/image_layout.hpp>
#include <gct/property.hpp>

namespace gct {
  vk::ImageViewType to_image_view_type( vk::ImageType v, std::uint32_t, bool = false );
  class image_view_create_info_t;
  class device_t;
  class subview_range;
  class image_t :
    public property< image_create_info_t > {
  public:
    image_t(
      const image_create_info_t&
    );
    virtual ~image_t() {}
    [[nodiscard]] virtual std::shared_ptr< image_view_t > get_view(
      const image_view_create_info_t&
    ) = 0;
    [[nodiscard]] virtual std::shared_ptr< image_view_t > get_view(
      vk::ImageAspectFlags aspect,
      bool force_array = false
    ) = 0;
    [[nodiscard]] virtual std::vector< std::shared_ptr< image_view_t > > get_thin_views(
      vk::ImageAspectFlags aspect,
      std::uint32_t layer,
      bool force_array = false
    ) = 0;
    [[nodiscard]] virtual std::shared_ptr< image_view_t > get_view(
      bool force_array = false
    ) = 0;
    [[nodiscard]] virtual std::vector< std::shared_ptr< image_view_t > > get_thin_views(
      std::uint32_t layer,
      bool force_array = false
    ) = 0;
    [[nodiscard]] virtual std::shared_ptr< image_view_t > get_view(
      const subview_range &
    ) = 0;
    [[nodiscard]] virtual vk::Image &operator*() = 0;
    [[nodiscard]] virtual const vk::Image &operator*() const = 0;
    [[nodiscard]] virtual vk::Image *operator->() = 0;
    [[nodiscard]] virtual const vk::Image *operator->() const = 0;
    [[nodiscard]] virtual std::shared_ptr< device_t >get_device() const = 0;
    [[nodiscard]] const image_layout_t &get_layout() const {
      return layout;
    };
    [[nodiscard]] image_layout_t &get_layout() {
      return layout;
    };
  protected:
    image_layout_t layout;
  };

  [[nodiscard]] std::vector< std::shared_ptr< image_view_t > >
  get_views(
    const std::vector< std::shared_ptr< image_t > > &
  );

void to_json( nlohmann::json&, const image_t& );

}

#endif

