#ifndef GCT_IMAGE_FILL_CREATE_INFO_HPP
#define GCT_IMAGE_FILL_CREATE_INFO_HPP

#include <string>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/image_pool.hpp>
#include <gct/color.hpp>

namespace gct {
struct image_fill_create_info {
  LIBGCT_SETTER( name )
  LIBGCT_SETTER( color )
  image_fill_create_info &set_output(
    const std::string &n,
    const image_allocate_info &desc
  ) {
    auto basic = desc.create_info.get_basic();
    basic.setUsage(
      basic.usage |
      vk::ImageUsageFlagBits::eTransferSrc |
      vk::ImageUsageFlagBits::eTransferDst |
      vk::ImageUsageFlagBits::eStorage |
      vk::ImageUsageFlagBits::eSampled
    );
    auto desc_ = desc;
    desc_.create_info.set_basic( basic );
    desc_.set_layout( vk::ImageLayout::eGeneral );
    output = desc_;
    name = n;
    return *this;
  }
  image_fill_create_info &set_output(
    const std::string &name,
    unsigned int width,
    unsigned int height
  ) {
    return set_output(
      name,
      gct::image_allocate_info()
        .set_create_info(
          gct::image_create_info_t()
            .set_basic(
              gct::basic_2d_image(
                width,
                height
              )
            )
        )
    );
  }
  image_fill_create_info &set_output(
    const image_allocate_info &desc
  ) {
    auto basic = desc.create_info.get_basic();
    basic.setUsage(
      basic.usage |
      vk::ImageUsageFlagBits::eTransferSrc |
      vk::ImageUsageFlagBits::eTransferDst |
      vk::ImageUsageFlagBits::eStorage |
      vk::ImageUsageFlagBits::eSampled
    );
    auto desc_ = desc;
    desc_.create_info.set_basic( basic );
    desc_.set_layout( vk::ImageLayout::eGeneral );
    output = desc_;
    return *this;
  }
  image_fill_create_info &set_output(
    unsigned int width,
    unsigned int height
  ) {
    return set_output(
      gct::image_allocate_info()
        .set_create_info(
          gct::image_create_info_t()
            .set_basic(
              gct::basic_2d_image(
                width,
                height
              )
            )
        )
    );
  }
  image_fill_create_info &set_output(
    const image_pool::image_descriptor &desc
  ) {
    output = desc;
    return *this;
  }
  std::string name = "default";
  std::variant< image_pool::image_descriptor, image_allocate_info > output;
  std::array< float, 4u > color = color::web::pink;
  bool independent = true;
};

void to_json( nlohmann::json&, const image_fill_create_info& );

}

#endif

