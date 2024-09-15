#ifndef GCT_CREATE_IMAGE_FROM_FILE_HPP
#define GCT_CREATE_IMAGE_FROM_FILE_HPP

#include <memory>
#include <tuple>
#include <string>
#include <vulkan/vulkan.hpp>
#include <gct/numeric_types.hpp>
#include <gct/color_space.hpp>

namespace gct {
class buffer_t;
class image_t;
class allocator_t;
std::tuple< std::shared_ptr< buffer_t >, std::shared_ptr< image_t >, std::shared_ptr< image_t > >
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

