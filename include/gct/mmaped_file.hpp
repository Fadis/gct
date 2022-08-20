#ifndef GCT_MAPPED_FILE_H
#define GCT_MAPPED_FILE_H
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
#include <cstddef>
#include <memory>
#include <filesystem>
#include <string>
namespace gct {
  class mmaped_file {
  public:
    using value_type = std::uint8_t;
    using reference = const value_type&;
    using rconst_eference = const value_type&;
    using iterator = const std::uint8_t*;
    using const_iterator = const std::uint8_t*;
    using size_type = std::size_t;
    using difference_type = std::ptrdiff_t;
    mmaped_file( const std::filesystem::path& );
    const_iterator begin() const { return head.get(); }
    const_iterator end() const { return std::next( head.get(), file_size ); }
    const_iterator cbegin() const { return head.get(); }
    const_iterator cend() const { return std::next( head.get(), file_size ); }
    size_type size() const { return file_size; }
    bool empty() const { return file_size == 0u; }
    reference at( size_type i ) const;
    reference operator[]( size_type i ) const {
      return head.get()[ i ];
    }
  private:
    std::size_t file_size = 0u;
    std::shared_ptr< std::uint8_t > head;
  };
}
#endif

