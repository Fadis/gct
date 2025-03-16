#ifndef GCT_FORMAT_HPP
#define GCT_FORMAT_HPP
#include <vulkan/vulkan.hpp>
#include <OpenImageIO/imageio.h>
#include <gct/numeric_types.hpp>
#include <gct/get_extensions.hpp>
#include <vulkan/vulkan_enums.hpp>
namespace gct {
  [[nodiscard]] vk::Flags<vk::ImageAspectFlagBits> format_to_aspect( vk::Format format );
  [[nodiscard]] unsigned int format_to_channels( vk::Format format );
  [[nodiscard]] unsigned int format_to_size( vk::Format format );
  [[nodiscard]] OIIO_NAMESPACE::TypeDesc format_to_type( vk::Format format );
  [[nodiscard]] bool is_rgba( vk::Format format );
  [[nodiscard]] bool is_bgra( vk::Format format );
  [[nodiscard]] unsigned int pixel_per_element_x( vk::Format format );
  [[nodiscard]] unsigned int pixel_per_element_y( vk::Format format );
  [[nodiscard]] std::vector< vk::Format > get_all_formats(
    std::uint32_t api_version,
    const extension_map_t &available_extensions
  );
  [[nodiscard]] OIIO_NAMESPACE::TypeDesc get_component_type( vk::Format format );
  [[nodiscard]] numeric_type_t format2numeric_type( vk::Format format );
  [[nodiscard]] bool is_signed( vk::Format format );
  [[nodiscard]] bool is_normalized( vk::Format format );
  [[nodiscard]] bool is_scaled( vk::Format format );
  [[nodiscard]] bool is_srgb( vk::Format format );
  [[nodiscard]] unsigned int format_to_component_size( vk::Format format );
  [[nodiscard]] numeric_component_type_t format_to_component_type( vk::Format format );
  [[nodiscard]] bool is_available_for( vk::ImageUsageFlags, vk::FormatFeatureFlags );
}

#endif

