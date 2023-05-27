#ifndef LIBGCT_INCLUDE_GCT_FONT_HPP
#define LIBGCT_INCLUDE_GCT_FONT_HPP
#include <cstdint>
#include <filesystem>
#include <memory>
#include <unordered_map>
namespace gct {
  class device_t;
  class command_buffer_recorder_t;
  class allocator_t;
  class buffer_t;
  struct invalid_font{};

  using glyph_id = std::uint32_t;
  struct glyph_index {
    std::uint32_t vertex_offset;
    std::uint32_t vertex_count;
  };
  using glyph_index_hash = std::unordered_map< glyph_id, glyph_index >;

  struct font {
    std::shared_ptr< buffer_t > buffer;
    glyph_index_hash index;
    std::uint32_t offset = 0u;
  };
}
#endif

