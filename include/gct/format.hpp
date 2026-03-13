#ifndef GCT_FORMAT_HPP
#define GCT_FORMAT_HPP
#include <vector>
#include <vulkan/vulkan.hpp>
#include <OpenImageIO/imageio.h>
#include <gct/numeric_types.hpp>
#include <gct/get_extensions.hpp>
#include <vulkan/vulkan_enums.hpp>
#if OIIO_VERSION_MAJOR <= 2 || ( OIIO_VERSION_MAJOR == 3 && OIIO_VERSION_MINOR == 0 )
#define OIIO_CURRENT_NAMESPACE OIIO_NAMESPACE
#endif
namespace gct {
  [[nodiscard]] vk::Flags<vk::ImageAspectFlagBits> format_to_aspect( vk::Format format );
  [[nodiscard]] unsigned int format_to_channels( vk::Format format );
  [[nodiscard]] unsigned int format_to_size( vk::Format format );
  [[nodiscard]] unsigned int format_to_block_width( vk::Format format );
  [[nodiscard]] unsigned int format_to_block_height( vk::Format format );
  [[nodiscard]] std::uint32_t get_subimage_bytes(
    vk::Format format,
    std::uint32_t width,
    std::uint32_t height,
    std::uint32_t mip
  );
  [[nodiscard]] OIIO_CURRENT_NAMESPACE::TypeDesc format_to_type( vk::Format format );
  [[nodiscard]] bool is_rgba( vk::Format format );
  [[nodiscard]] bool is_bgra( vk::Format format );
  [[nodiscard]] unsigned int pixel_per_element_x( vk::Format format );
  [[nodiscard]] unsigned int pixel_per_element_y( vk::Format format );
  [[nodiscard]] std::vector< vk::Format > get_all_formats(
    std::uint32_t api_version,
    const extension_map_t &available_extensions
  );
  [[nodiscard]] OIIO_CURRENT_NAMESPACE::TypeDesc get_component_type( vk::Format format );
  [[nodiscard]] numeric_type_t format2numeric_type( vk::Format format );
  [[nodiscard]] bool is_signed( vk::Format format );
  [[nodiscard]] bool is_normalized( vk::Format format );
  [[nodiscard]] bool is_scaled( vk::Format format );
  [[nodiscard]] bool is_srgb( vk::Format format );
  [[nodiscard]] unsigned int format_to_component_size( vk::Format format );
  [[nodiscard]] numeric_component_type_t format_to_component_type( vk::Format format );
  [[nodiscard]] bool is_available_for( vk::ImageUsageFlags, vk::FormatFeatureFlags );
  [[nodiscard]] std::vector< vk::Format > get_compatible_format( vk::Format f );
  [[nodiscard]] vk::Format add_srgb( vk::Format f );
  [[nodiscard]] vk::Format remove_srgb( vk::Format f );
  [[nodiscard]] bool is_hdr( vk::Format );
}

#endif

