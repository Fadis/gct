#ifndef GCT_CREATE_IMAGE_FROM_FILE_HPP
#define GCT_CREATE_IMAGE_FROM_FILE_HPP

#include <memory>
#include <string>
#include <vulkan/vulkan.hpp>
#include <gct/numeric_types.hpp>
#include <gct/color_space.hpp>
#include <gct/setter.hpp>

namespace gct {
class buffer_t;
class image_t;
class allocator_t;

struct create_image_from_file_state {
  LIBGCT_SETTER( loaded )
  LIBGCT_SETTER( staging_buffer )
  LIBGCT_SETTER( nonlinear_image )
  LIBGCT_SETTER( linear_image )
  LIBGCT_SETTER( generate_mipmap )
  LIBGCT_SETTER( copy_range )
  create_image_from_file_state &add_copy_range(
    const vk::BufferImageCopy &r
  ) {
    copy_range.push_back( r );
    return *this;
  }
  create_image_from_file_state &clear_copy_range() {
    copy_range.clear();
    return *this;
  }
  bool loaded = false;
  std::shared_ptr< buffer_t > staging_buffer;
  std::shared_ptr< image_t > nonlinear_image;
  std::shared_ptr< image_t > linear_image;
  bool generate_mipmap = false;
  std::vector< vk::BufferImageCopy > copy_range;
};

create_image_from_file_state
create_image_from_file(
  const std::shared_ptr< allocator_t > &allocator,
  const std::string &filename,
  vk::ImageUsageFlags usage,
  bool mipmap,
  integer_attribute_t attr,
  unsigned int max_channels_per_layer,
  bool enable_srgb = true
);
}

#endif

