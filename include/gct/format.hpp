#ifndef GCT_FORMAT_HPP
#define GCT_FORMAT_HPP
#include <vulkan/vulkan.hpp>
#include <OpenImageIO/imageio.h>
#include <gct/numeric_types.hpp>
namespace gct {
  vk::Flags<vk::ImageAspectFlagBits> format_to_aspect( vk::Format format );
  unsigned int format_to_channels( vk::Format format );
  unsigned int format_to_size( vk::Format format );
  OIIO_NAMESPACE::TypeDesc format_to_type( vk::Format format );
  bool is_rgba( vk::Format format );
  bool is_bgra( vk::Format format );
  unsigned int pixel_per_element_x( vk::Format format );
  unsigned int pixel_per_element_y( vk::Format format );
  const std::vector< vk::Format > &get_all_formats();
  OIIO_NAMESPACE::TypeDesc get_component_type( vk::Format format );
  numeric_type_t format2numeric_type( vk::Format format );
  bool is_signed( vk::Format format );
  bool is_normalized( vk::Format format );
  bool is_scaled( vk::Format format );
  bool is_srgb( vk::Format format );
  unsigned int format_to_component_size( vk::Format format );
  numeric_component_type_t format_to_component_type( vk::Format format );
}

#endif

