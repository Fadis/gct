#include <gct/format.hpp>

namespace gct {

std::uint32_t get_subimage_bytes(
  vk::Format format,
  std::uint32_t width,
  std::uint32_t height,
  std::uint32_t mip
) {
  const std::uint32_t uncompressed_width = width >> mip;
  const std::uint32_t uncompressed_height = height >> mip;
  const std::uint32_t block_width = format_to_block_width( format );
  const std::uint32_t block_height = format_to_block_height( format );
  const std::uint32_t x_block_count = uncompressed_width / block_width + ( ( uncompressed_width % block_width ) ? 1u : 0u );
  const std::uint32_t y_block_count = uncompressed_height / block_height + ( ( uncompressed_height % block_height ) ? 1u : 0u );
  const std::uint32_t block_bytes = format_to_size( format );
  return x_block_count * y_block_count * block_bytes;
}

}

