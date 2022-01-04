#ifndef GCT_FORMAT_HPP
#define GCT_FORMAT_HPP
#include <vulkan/vulkan.hpp>
#include <OpenImageIO/imageio.h>

namespace gct {
  vk::Flags<vk::ImageAspectFlagBits> format_to_aspect( vk::Format format );
  unsigned int format_to_channels( vk::Format format );
  unsigned int format_to_size( vk::Format format );
  OIIO_NAMESPACE::TypeDesc format_to_type( vk::Format format );
  bool is_rgba( vk::Format format );
  bool is_bgra( vk::Format format );
  unsigned int pixel_per_element_x( vk::Format format );
  unsigned int pixel_per_element_y( vk::Format format );
}

#endif

