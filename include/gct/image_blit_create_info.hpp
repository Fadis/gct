#ifndef GCT_IMAGE_BLIT_CREATE_INFO_HPP
#define GCT_IMAGE_BLIT_CREATE_INFO_HPP

#include <string>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/image_pool.hpp>
#include <gct/dynamic_size_image_allocate_info.hpp>

namespace gct {

struct image_blit_create_info {
  LIBGCT_SETTER( node_name )
  LIBGCT_SETTER( input_name )
  LIBGCT_SETTER( output_name )
  LIBGCT_SETTER( region )
  LIBGCT_SETTER( filter )
  LIBGCT_SETTER( shareable )
  image_blit_create_info &set_output(
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
    output_name = n;
    return *this;
  }
  image_blit_create_info &set_output(
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
  image_blit_create_info &set_output(
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
  image_blit_create_info &set_output(
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
  image_blit_create_info &set_output(
    const image_pool::image_descriptor &desc
  ) {
    output = desc;
    return *this;
  }
  image_blit_create_info &add_region(
    const vk::ImageBlit &r
  ) {
    region.push_back( r );
    return *this;
  }
  std::string input_name = "input";
  std::string output_name = "output";
  image_pool::image_descriptor input;
  std::variant< image_pool::image_descriptor, image_allocate_info, dynamic_size_image_allocate_info > output;
  std::vector< vk::ImageBlit > region;
  vk::Filter filter = vk::Filter::eLinear;
  bool independent = true;
  bool shareable = true;
  std::string node_name;
};

void to_json( nlohmann::json&, const image_blit_create_info& );

}

#endif

