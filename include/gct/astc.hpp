#ifndef GCT_ASTC_H
#define GCT_ASTC_H
/*
 * Copyright (C) 2022 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <cstdint>
#include <utility>
#include <vulkan/vulkan.hpp>

namespace gct {
  struct astc_header {
    std::uint8_t magic[4];
    std::uint8_t block_x;
    std::uint8_t block_y;
    std::uint8_t block_z;
    std::uint8_t dim_x[3];
    std::uint8_t dim_y[3];
    std::uint8_t dim_z[3];
  };
  bool is_astc( const astc_header &header );
  vk::Format get_format( const astc_header &header, bool srgb );
  std::pair< std::uint32_t, std::uint32_t > get_dims( const astc_header &header );
  std::uint32_t get_size( const astc_header &header );
}
#endif

