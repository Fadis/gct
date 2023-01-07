#ifndef LIBGCT_TEXT_LAYOUT_ENGINE_HPP
#define LIBGCT_TEXT_LAYOUT_ENGINE_HPP
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
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <string>
#include <array>
#include <vector>
#include <algorithm>
#include <unicode/ustring.h>
#include <unicode/ubidi.h>
#include <unicode/ucsdet.h>
#include <unicode/unorm2.h>
#include <unicode/uscript.h>
#include <unicode/ubrk.h>
#include <hb.h>

namespace gct::text {

struct segment {
  bool breakable = false;
  std::uint32_t script_code = 0u;
  bool newline = false;
  std::size_t begin = 0u;
  std::size_t end = 0u;
};

struct run {
  bool is_rtl = false;
  std::u32string str;
  std::vector< segment > segments;
};

using paragraph = std::vector< run >;

std::string to_utf8( const std::u32string &v );

std::u32string get_segment( const run &b, const segment &s );
std::pair< paragraph, bool > segment_string( const UChar32 *utf32, std::int32_t utf32_length, const char *locale, bool char_break );
std::pair< paragraph, bool > segment_string( const char *utf8, std::int32_t utf8_length, const char *locale, bool char_break );
std::uint32_t count_newline( const run &b, const segment &s );

struct glyph {
  std::uint32_t glyph_id = 0.f;
  std::uint32_t script_code = 0u;
  bool breakable = false;
  bool is_rtl = false;
  float pxpem = 0.f;
  hb_position_t x_offset = 0.f;
  hb_position_t y_offset = 0.f;
  hb_position_t x_advance = 0.f;
  hb_position_t y_advance = 0.f;
};

struct line {
  bool is_rtl = false;
  std::vector< glyph > glyphs;
};

struct glyph_pos {
  std::uint32_t glyph_id = 0.f;
  float x_pos;
  float y_pos;
  float size;
};

struct text_pos {
  bool is_rtl = false;
  float height = 0.f;
  float cursor_x = 0.f;
  float cursor_y = 0.f;
  std::vector< glyph_pos > glyphs;
};

enum class line_direction {
  UNKNOWN = 0,
  LTR = 1,
  RTL = 2
};

class layout_engine final {
public:
  layout_engine(
    const char *filename,
    float pxpem_,
    float area_width_,
    float line_spacing_
  );
  ~layout_engine();
  layout_engine( const layout_engine& ) = delete;
  layout_engine( layout_engine&& ) = delete;
  layout_engine &operator=( const layout_engine& ) = delete;
  layout_engine &operator=( layout_engine&& ) = delete;
  text_pos layout( const std::vector< line > &lines, float cursor_x=0.f, float cursor_y=0.f );
  std::int32_t split_ltr( const glyph *begin, std::int32_t len, float area_width );
  std::int32_t split_rtl( const glyph *begin, std::int32_t len, float area_width );
  std::vector< line > wrap_line( const std::vector< line > &ls );
  std::vector< line > wrap_line( const line &l );
  std::vector< line > parse_lines( const char *utf8, std::uint32_t utf8_length, const char *locale );
  line parse_line( const char *utf8, std::uint32_t utf8_length, const char *locale );
private:
  hb_blob_t *blob;
  hb_face_t *face;
  hb_font_t *font;
  float pxpem;
  float area_width;
  float line_spacing;
};

UChar32 get_hyphen( UChar32 v );

}

#endif

