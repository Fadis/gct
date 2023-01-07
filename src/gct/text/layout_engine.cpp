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
#include <gct/text/layout_engine.hpp>

namespace gct::text {

namespace {
bool is_newline( UChar32 v ) {
  return  v == 0x000A || v == 0x000D;
}

std::pair< paragraph, bool > segment_short( const UChar *utf16nfc, std::int32_t utf16nfc_length, const char *locale, bool char_break ) {
  UErrorCode error = U_ZERO_ERROR;

  const auto base_direction = ubidi_getBaseDirection( utf16nfc, utf16nfc_length );

  auto bidi = ubidi_open();
  if( !bidi ) std::abort();

  ubidi_setPara( bidi, utf16nfc, utf16nfc_length, base_direction == UBIDI_RTL, nullptr, &error );
  if( U_FAILURE( error ) ) std::abort();

  const auto run_count = ubidi_countRuns( bidi, &error );

  paragraph para;

  for( unsigned int i = 0u; i != run_count; ++i ) {
    int32_t start = 0u;
    int32_t length = 0u;
    auto bidi_dir = ubidi_getVisualRun( bidi, i, &start, &length );
    if( bidi_dir == UBIDI_NEUTRAL ) {
      std::abort();
    }
    if( bidi_dir == UBIDI_MIXED ) {
      std::abort();
    }

    bool is_rtl = ( UBIDI_RTL == bidi_dir );

    auto iter = ubrk_open( char_break ? UBRK_CHARACTER : UBRK_LINE, locale, utf16nfc + start, length, &error );
   
    para.push_back( run{} );
    para.back().is_rtl = is_rtl;

    std::array< UChar32, 512u > utf32 = { 0 };
    int32_t cur = 0u;
    int32_t next;
    while( ( next = ubrk_next( iter ) ) != UBRK_DONE ) {
      std::int32_t utf32_length = 0u;
      u_strToUTF32( utf32.data(), utf32.size(), &utf32_length, utf16nfc + start + cur, next - cur, &error );
      if( U_FAILURE( error ) ) std::abort();

      if( utf32_length != 0u ) {
        auto script_code = uscript_getScript( utf32[ 0 ], &error );
        bool newline = is_newline( utf32[ 0 ] );
        if( U_FAILURE( error ) ) std::abort();
        para.back().segments.push_back(
          segment{
            true,
            std::uint32_t( script_code ),
            newline,
            para.back().str.size(),
            0u
          }
        );
        for(
          std::uint32_t i = 1u;
          i != utf32_length;
          ++i
        ) {
          const auto current_script_code = uscript_getScript( utf32[ i ], &error );
          bool current_newline = is_newline( utf32[ i ] );
          if( U_FAILURE( error ) ) std::abort();
          if( current_newline != newline || current_script_code != script_code ) {
            script_code = current_script_code;
            newline = current_newline;
            para.back().segments.back().end = para.back().str.size() + i;
            para.back().segments.push_back(
              segment{
                false,
                std::uint32_t( script_code ),
                newline,
                para.back().str.size() + i,
                0u
              }
            );
          }
        }
        para.back().segments.back().end = para.back().str.size() + utf32_length;
      }
      std::transform(
        utf32.data(),
        std::next( utf32.data(), utf32_length ),
        std::back_inserter( para.back().str ),
        []( UChar32 v ) -> char32_t {
          return char32_t( v );
        }
      );
      cur = next;
    }
    if( cur != length ) {
      next = utf16nfc_length;
      std::int32_t utf32_length = 0u;
      u_strToUTF32( utf32.data(), utf32.size(), &utf32_length, utf16nfc + start + cur, next - cur, &error );
      if( U_FAILURE( error ) ) std::abort();
      
      if( utf32_length != 0u ) {
        auto script_code = uscript_getScript( utf32[ 0 ], &error );
        bool newline = is_newline( utf32[ 0 ] );
        if( U_FAILURE( error ) ) std::abort();
        para.back().segments.push_back(
          segment{
            true,
            std::uint32_t( script_code ),
            newline,
            para.back().str.size(),
            0u
          }
        );
        for(
          std::uint32_t i = 1u;
          i != utf32_length;
          ++i
        ) {
          const auto current_script_code = uscript_getScript( utf32[ i ], &error );
          bool current_newline = is_newline( utf32[ i ] );
          if( U_FAILURE( error ) ) std::abort();
          if( current_script_code != script_code ) {
            script_code = current_script_code;
            newline = current_newline;
            para.back().segments.back().end = para.back().str.size() + i;
            para.back().segments.push_back(
              segment{
                false,
                std::uint32_t( script_code ),
                newline,
                para.back().str.size() + i,
                0u
              }
            );
          }
        }
        para.back().segments.back().end = para.back().str.size() + utf32_length;
      }
      std::transform(
        utf32.data(),
        std::next( utf32.data(), utf32_length ),
        std::back_inserter( para.back().str ),
        []( UChar32 v ) -> char32_t {
          return char32_t( v );
        }
      );
    }
    ubrk_close( iter );
  }
  ubidi_close( bidi );
  return std::make_pair( para, base_direction == UBIDI_RTL );
}

std::pair< paragraph, bool > segment_short( const char *utf8, std::uint32_t utf8_length, const char *locale, bool char_break ) {
  UErrorCode error = U_ZERO_ERROR;
  
  std::array< UChar, 512u > utf16 = { 0 };
  std::int32_t utf16_length = 0u;
  u_strFromUTF8( utf16.data(), utf16.size(), &utf16_length, utf8, utf8_length, &error );
  if( U_FAILURE( error ) ) std::abort();

  auto norm = unorm2_getNFCInstance( &error );
  if( U_FAILURE( error ) ) std::abort();

  std::array< UChar, 512u > utf16nfc = { 0 };
  const auto utf16nfc_length = unorm2_normalize( norm, utf16.data(), utf16_length, utf16nfc.data(), utf16nfc.size(), &error );
  if( U_FAILURE( error ) ) std::abort();

  return segment_short( utf16nfc.data(), utf16nfc_length, locale, char_break );
}

std::pair< paragraph, bool > segment_short( const UChar32 *utf32, std::int32_t utf32_length, const char *locale, bool char_break ) {
  UErrorCode error = U_ZERO_ERROR;

  std::array< UChar, 512u > utf16 = { 0 };
  std::int32_t utf16_length = 0u;
  u_strFromUTF32( utf16.data(), utf16.size(), &utf16_length, utf32, utf32_length, &error );
  if( U_FAILURE( error ) ) std::abort();

  return segment_short( utf16.data(), utf16_length, locale, char_break );
}

std::pair< paragraph, bool > segment_long( const UChar *utf16nfc, std::int32_t utf16nfc_length, const char *locale, bool char_break ) {
  UErrorCode error = U_ZERO_ERROR;

  const auto base_direction = ubidi_getBaseDirection( utf16nfc, utf16nfc_length );
  
  auto bidi = ubidi_open();
  if( !bidi ) std::abort();

  ubidi_setPara( bidi, utf16nfc, utf16nfc_length, base_direction == UBIDI_RTL, nullptr, &error );
  if( U_FAILURE( error ) ) std::abort();

  const auto run_count = ubidi_countRuns( bidi, &error );

  paragraph para;

  for( unsigned int i = 0u; i != run_count; ++i ) {
    int32_t start = 0u;
    int32_t length = 0u;
    bool is_rtl = ( UBIDI_RTL == ubidi_getVisualRun( bidi, i, &start, &length ) );

    
    auto iter = ubrk_open( char_break ? UBRK_CHARACTER : UBRK_LINE, locale, utf16nfc + start, length, &error );
    if( U_FAILURE( error ) ) std::abort();
   
    para.push_back( run{} );
    para.back().is_rtl = is_rtl;

    std::vector< UChar32 > utf32;
    int32_t cur = 0u;
    int32_t next;
    while( ( next = ubrk_next( iter ) ) != UBRK_DONE ) {
      std::int32_t utf32_length = 0u;
      utf32.resize( next - cur + 1u, 0u );
      u_strToUTF32( utf32.data(), utf32.size(), &utf32_length, utf16nfc + start + cur, next - cur, &error );
      if( U_FAILURE( error ) ) std::abort();
      utf32.resize( utf32_length );
      
      if( utf32_length != 0u ) {
        auto script_code = uscript_getScript( utf32[ 0 ], &error );
        bool newline = is_newline( utf32[ 0 ] );
        if( U_FAILURE( error ) ) std::abort();
        para.back().segments.push_back(
          segment{
            true,
            std::uint32_t( script_code ),
            newline,
            para.back().str.size(),
            0u
          }
        );
        for(
          std::uint32_t i = 1u;
          i != utf32_length;
          ++i
        ) {
          const auto current_script_code = uscript_getScript( utf32[ i ], &error );
          bool current_newline = is_newline( utf32[ i ] );
          if( U_FAILURE( error ) ) std::abort();
          if( current_newline != newline || current_script_code != script_code ) {
            script_code = current_script_code;
            newline = current_newline;
            para.back().segments.back().end = para.back().str.size() + i;
            para.back().segments.push_back(
              segment{
                false,
                std::uint32_t( script_code ),
                newline,
                para.back().str.size() + i,
                0u
              }
            );
          }
        }
        para.back().segments.back().end = para.back().str.size() + utf32_length;
      }
      std::transform(
        utf32.data(),
        std::next( utf32.data(), utf32_length ),
        std::back_inserter( para.back().str ),
        []( UChar32 v ) -> char32_t {
          return char32_t( v );
        }
      );
      cur = next;
    }
    if( cur != length ) {
      next = utf16nfc_length;
      std::int32_t utf32_length = 0u;
      utf32.resize( next - cur + 1u, 0u );
      u_strToUTF32( utf32.data(), utf32.size(), &utf32_length, utf16nfc + start + cur, next - cur, &error );
      if( U_FAILURE( error ) ) std::abort();
      utf32.resize( utf32_length );
      
      if( utf32_length != 0u ) {
        auto script_code = uscript_getScript( utf32[ 0 ], &error );
        bool newline = is_newline( utf32[ 0 ] );
        if( U_FAILURE( error ) ) std::abort();
        para.back().segments.push_back(
          segment{
            true,
            std::uint32_t( script_code ),
            newline,
            para.back().str.size(),
            0u
          }
        );
        for(
          std::uint32_t i = 1u;
          i != utf32_length;
          ++i
        ) {
          auto current_script_code = uscript_getScript( utf32[ i ], &error );
          bool current_newline = is_newline( utf32[ i ] );
          if( U_FAILURE( error ) ) std::abort();
          if( current_newline != newline || current_script_code != script_code ) {
            script_code = current_script_code;
            newline = current_newline;
            para.back().segments.back().end = para.back().str.size() + i;
            para.back().segments.push_back(
              segment{
                false,
                std::uint32_t( script_code ),
                newline,
                para.back().str.size() + i,
                0u
              }
            );
          }
        }
        para.back().segments.back().end = para.back().str.size() + utf32_length;
      }
      std::transform(
        utf32.data(),
        std::next( utf32.data(), utf32_length ),
        std::back_inserter( para.back().str ),
        []( UChar32 v ) -> char32_t {
          return char32_t( v );
        }
      );
    }
    ubrk_close( iter );
  }
  ubidi_close( bidi );
  return std::make_pair( para, base_direction == UBIDI_RTL );
}

std::pair< paragraph, bool > segment_long( const char *utf8, std::uint32_t utf8_length, const char *locale, bool char_break ) {
  UErrorCode error = U_ZERO_ERROR;
  
  std::vector< UChar > utf16( utf8_length+1u, 0 );
  std::int32_t utf16_length = 0u;
  u_strFromUTF8( utf16.data(), utf16.size(), &utf16_length, utf8, utf8_length, &error );
  if( U_FAILURE( error ) ) std::abort();
  utf16.resize( utf16_length );

  auto norm = unorm2_getNFCInstance( &error );
  if( U_FAILURE( error ) ) std::abort();

  std::vector< UChar > utf16nfc( utf16.size()+1u, 0 );
  const auto utf16nfc_length = unorm2_normalize( norm, utf16.data(), utf16_length, utf16nfc.data(), utf16nfc.size(), &error );
  if( U_FAILURE( error ) ) std::abort();
  utf16.resize( utf16nfc_length );

  return segment_long( utf16nfc.data(), utf16nfc_length, locale, char_break );
}

std::pair< paragraph, bool > segment_long( const UChar32 *utf32, std::int32_t utf32_length, const char *locale, bool char_break ) {
  UErrorCode error = U_ZERO_ERROR;

  std::vector< UChar > utf16( utf32_length*2u+1u, 0 );
  std::int32_t utf16_length = 0u;
  u_strFromUTF32( utf16.data(), utf16.size(), &utf16_length, utf32, utf32_length, &error );
  if( U_FAILURE( error ) ) std::abort();
  utf16.resize( utf16_length );

  return segment_long( utf16.data(), utf16_length, locale, char_break );
}

}

std::uint32_t count_newline( const run &b, const segment &s ) {
  const auto end = std::next( b.str.begin(), s.end );
  bool cr = false;
  std::uint32_t count = 0u;
  for(
    auto iter = std::next( b.str.begin(), s.begin );
    iter != end;
    ++iter
  ) {
    if( *iter == 0x000D ) {
      cr = true;
      ++count;
    }
    else if( *iter == 0x000A ) {
      if( !cr ) ++count;
      cr = false;
      ++count;
    }
    else cr = false;
  }
  return count;
}

std::u32string get_segment( const run &b, const segment &s ) {
  return b.str.substr( s.begin, s.end - s.begin );
}

std::pair< paragraph, bool > segment_string( const UChar32 *utf32, std::int32_t utf32_length, const char *locale, bool char_break ) {
  if( utf32_length < 256u ) return segment_short( utf32, utf32_length, locale, char_break );
  else return segment_long( utf32, utf32_length, locale, char_break );
}


std::pair< paragraph, bool > segment_string( const char *utf8, std::int32_t utf8_length, const char *locale, bool char_break ) {
  if( utf8_length < 512u ) return segment_short( utf8, utf8_length, locale, char_break );
  else return segment_long( utf8, utf8_length, locale, char_break );
}

layout_engine::layout_engine(
  const char *filename,
  float pxpem_,
  float area_width_,
  float line_spacing_
) :
  pxpem( pxpem_ ),
  area_width( area_width_ ),
  line_spacing( line_spacing_ ) {
  blob = hb_blob_create_from_file( filename );
  face = hb_face_create(blob, 0);
  font = hb_font_create(face);
}
layout_engine::~layout_engine() {
  hb_font_destroy(font);
  hb_face_destroy(face);
  hb_blob_destroy(blob);
}
text_pos layout_engine::layout( const std::vector< line > &lines, float cursor_x, float cursor_y ) {
  text_pos pos;
  float v_pos_base = cursor_y;
  float h_pos = cursor_x;
  float v_pos = v_pos_base;
  for( const auto &line: lines ) {
    const auto largest_glyph = std::max_element( line.glyphs.begin(), line.glyphs.end(), []( auto l, auto r ){ return l.pxpem < r.pxpem; } );
    const auto max_pxpem = ( largest_glyph != line.glyphs.end() ) ? largest_glyph->pxpem : pxpem;
    v_pos_base -= max_pxpem + line_spacing;
    h_pos = 0;
    v_pos = v_pos_base;
    const std::int32_t head = pos.glyphs.size();
    for( const auto &g: line.glyphs ) {
      float glyph_h_pos = h_pos + g.pxpem * g.x_offset / 1024.f;
      float glyph_v_pos = v_pos + g.pxpem * g.y_offset / 1024.f;
      pos.glyphs.push_back( glyph_pos{ g.glyph_id, glyph_h_pos, glyph_v_pos, g.pxpem } );
      h_pos += g.pxpem * g.x_advance / 1024.f;
      v_pos += g.pxpem * g.y_advance / 1024.f;
    }
    if( line.is_rtl ) {
      for( std::int32_t i = head; i != pos.glyphs.size(); ++i ) {
        pos.glyphs[ i ].x_pos += area_width - h_pos;
      }
    }
  }
  pos.cursor_x = h_pos;
  pos.cursor_y = v_pos;
  v_pos_base -= line_spacing;
  for( auto &g: pos.glyphs ) {
    g.y_pos -= v_pos_base;
  }
  pos.height = v_pos_base;
  return pos;
}
std::int32_t layout_engine::split_ltr( const glyph *begin, std::int32_t len, float area_width ) {
  float line_width = 0.f;
  std::int32_t saved = 0;
  std::int32_t checkpoint = 0;
  for( std::int32_t i = 0u; i != len; ) {
    auto glyph_width = begin[ i ].pxpem * begin[ i ].x_advance / 1024.f;
    if( line_width + glyph_width > area_width ) {
      return checkpoint;
    }
    else {
      if( begin[ i ].breakable && !begin[ i ].is_rtl ) {
        checkpoint = i;
      }
      line_width += glyph_width;
      ++i;
    }
  }
  return len;
}
std::int32_t layout_engine::split_rtl( const glyph *begin, std::int32_t len, float area_width ) {
  float line_width = 0.f;
  std::int32_t saved = len - 1;
  std::int32_t checkpoint = len - 1;
  for( std::int32_t i = len - 1; i >= 0; ) {
    auto glyph_width = begin[ i ].pxpem * begin[ i ].x_advance / 1024.f;
    if( line_width + glyph_width > area_width ) {
      return checkpoint + 1;
    }
    else {
      if( begin[ i ].breakable && begin[ i ].is_rtl ) {
        checkpoint = i;
      }
      line_width += glyph_width;
      --i;
    }
  }
  return 0;
}
std::vector< line > layout_engine::wrap_line( const std::vector< line > &ls ) {
  std::vector< line > dest;
  for( const auto &l : ls ) {
    auto partial = wrap_line( l );
    dest.insert( dest.end(), partial.begin(), partial.end() );
  }
  return dest;
}
std::vector< line > layout_engine::wrap_line( const line &l ) {
  std::vector< line > dest;
  std::int32_t k = 0;
  if( !l.is_rtl ) {
    float line_width = 0.f;
    std::int32_t saved = 0;
    std::int32_t checkpoint = 0;
    float ltr_width = 0.f;
    std::int32_t last_ltr = 0;
    for( std::int32_t i = 0u; i != l.glyphs.size(); ) {
      auto glyph_width = l.glyphs[ i ].pxpem * l.glyphs[ i ].x_advance / 1024.f;
      if( line_width + glyph_width > area_width ) {
        if( l.glyphs[ i ].is_rtl ) {
          std::int32_t rtl_end = i;
          for( ; rtl_end != l.glyphs.size(); ++rtl_end ) {
            if( !l.glyphs[ rtl_end ].is_rtl ) {
              break;
            }
          }
          const std::int32_t rtl_begin = split_rtl( std::next( l.glyphs.data(), last_ltr ), rtl_end - last_ltr, area_width - ltr_width ) + last_ltr;
          if( rtl_end != rtl_begin ) {
            line left;
            left.is_rtl = l.is_rtl;
            std::copy(
              std::next( l.glyphs.begin(), last_ltr ),
              std::next( l.glyphs.begin(), rtl_begin ),
              std::back_inserter( left.glyphs )
            );
            std::copy(
              std::next( l.glyphs.begin(), rtl_end ),
              std::next( l.glyphs.end() ),
              std::back_inserter( left.glyphs )
            );
            dest.push_back( line{
              l.is_rtl,
              std::vector< glyph >{
                std::next( l.glyphs.begin(), saved ),
                std::next( l.glyphs.begin(), last_ltr )
              }
            } );
            dest.back().glyphs.insert(
              dest.back().glyphs.end(),
              std::next( l.glyphs.begin(), rtl_begin ),
              std::next( l.glyphs.begin(), rtl_end )
            );
            auto left_splited = wrap_line( left );
            dest.insert(
              dest.end(),
              left_splited.begin(),
              left_splited.end()
            );
            return dest;
          }
        }
        if( saved == checkpoint ) {
          std::abort();
        }
        dest.push_back( line{
          l.is_rtl,
          std::vector< glyph >{
            std::next( l.glyphs.begin(), saved ),
            std::next( l.glyphs.begin(), checkpoint )
          }
        } );
        line_width = 0.;
        saved = checkpoint;
        i = checkpoint;
      }
      else {
        if( l.glyphs[ i ].breakable && !l.glyphs[ i ].is_rtl ) {
          checkpoint = i;
        }
        line_width += glyph_width;
        ++i;
        if( !l.glyphs[ i ].is_rtl ) {
          last_ltr = i;
          ltr_width = line_width;
        }
      }
    }
    dest.push_back( line{
      l.is_rtl,
      std::vector< glyph >{
        std::next( l.glyphs.begin(), saved ),
        l.glyphs.end()
      }
    } );
  }
  else {
    float line_width = 0.f;
    std::int32_t saved = l.glyphs.size() - 1u;
    std::int32_t checkpoint = l.glyphs.size() - 1u;
    float rtl_width = 0.f;
    std::int32_t last_rtl = 0;
    for( std::int32_t i = l.glyphs.size() - 1u; i >= 0; ) {
      auto glyph_width = l.glyphs[ i ].pxpem * l.glyphs[ i ].x_advance / 1024.f;
      if( line_width + glyph_width > area_width ) {
        if( !l.glyphs[ i ].is_rtl ) {
          std::int32_t ltr_begin = i;
          for( ; ltr_begin >= 0; --ltr_begin ) {
            if( l.glyphs[ ltr_begin ].is_rtl ) {
              break;
            }
          }
          ++ltr_begin;
          const std::int32_t ltr_end = split_ltr( std::next( l.glyphs.data(), ltr_begin ), last_rtl - ltr_begin, area_width - rtl_width ) + ltr_begin;
          if( ltr_end != ltr_begin ) {
            line left;
            left.is_rtl = l.is_rtl;
            std::copy(
              l.glyphs.begin(),
              std::next( l.glyphs.begin(), ltr_begin ),
              std::back_inserter( left.glyphs )
            );
            std::copy(
              std::next( l.glyphs.begin(), ltr_end ),
              std::next( l.glyphs.begin(), last_rtl ),
              std::back_inserter( left.glyphs )
            );
            dest.push_back( line{
              l.is_rtl,
              std::vector< glyph >{
                std::next( l.glyphs.begin(), ltr_begin ),
                std::next( l.glyphs.begin(), ltr_end )
              }
            } );
            dest.back().glyphs.insert(
              dest.back().glyphs.end(),
              std::next( l.glyphs.begin(), last_rtl ),
              std::next( l.glyphs.begin(), saved + 1u )
            );
            auto left_splited = wrap_line( left );
            dest.insert(
              dest.end(),
              left_splited.begin(),
              left_splited.end()
            );
            return dest;
          }
        }
        if( saved == checkpoint ) {
          std::abort();
        }
        dest.push_back( line{
          l.is_rtl,
          std::vector< glyph >{
            std::next( l.glyphs.begin(), checkpoint + 1u ),
            std::next( l.glyphs.begin(), saved + 1u )
          }
        } );
        line_width = 0.;
        saved = checkpoint;
        i = checkpoint;
      }
      else {
        if( l.glyphs[ i ].breakable && l.glyphs[ i ].is_rtl ) {
          checkpoint = i;
        }
        line_width += glyph_width;
        --i;
        if( l.glyphs[ i ].is_rtl ) {
          last_rtl = i;
          rtl_width = line_width;
        }
      }
    }
    dest.push_back( line{
      l.is_rtl,
      std::vector< glyph >{
        l.glyphs.begin(),
        std::next( l.glyphs.begin(), saved + 1u )
      }
    } );

  }
  return dest;
}
std::vector< line > layout_engine::parse_lines( const char *utf8, std::uint32_t utf8_length, const char *locale ) {
  std::vector< line > lines;
  auto cur = utf8;
  const auto end = std::next( utf8, utf8_length );
  while( cur != end ) {
    auto newline = std::find_if( cur, end, []( auto v ) { return v == 0x0D || v == 0x0A; } );
    bool initial = false;
    if( newline != cur ) {
      lines.push_back( parse_line( cur, std::distance( cur, newline ), locale ) );
      initial = true;
    }
    if( newline != end ) {
      auto non_newline = std::find_if( newline, end, []( auto v ) { return v != 0x0D && v != 0x0A; } );
      bool cr = false;
      for( auto iter = newline; iter != non_newline; ++iter ) {
        if( *iter == 0x0D ) {
          if( initial ) initial = false;
          lines.push_back( line{} );
          cr = true;
        }
        else if( *iter == 0x0A ) {
          if( cr ) {
            cr = false;
          }
          else {
            if( initial ) initial = false;
            else lines.push_back( line{} );
          }
        }
        else cr = false;
      }
      cur = non_newline;
    }
    else cur = newline;
  }
  return lines;
}
line layout_engine::parse_line( const char *utf8, std::uint32_t utf8_length, const char *locale ) {
  line dest;
  std::uint32_t ltr_cursor = 0u;
  std::uint32_t rtl_cursor = 0u;
  const auto [runs,is_rtl] = gct::text::segment_string( utf8, utf8_length, locale, false );
  dest.is_rtl = is_rtl;
  for( const auto &r : runs ) {
    for( std::int32_t sid = 0; sid != std::int32_t( r.segments.size() ); ++sid ) {
      const bool breakable = r.segments[ sid ].breakable;
      auto &s = r.segments[ sid ];
      assert( !s.newline );
      hb_buffer_t *buf = hb_buffer_create();
      hb_buffer_add_utf32( buf, reinterpret_cast< const std::uint32_t* >( r.str.data() + s.begin ), s.end - s.begin, 0u, s.end - s.begin );
      hb_buffer_set_direction( buf, r.is_rtl ? HB_DIRECTION_RTL : HB_DIRECTION_LTR );
      hb_buffer_set_script(buf, hb_script_t( s.script_code ) );
      hb_buffer_set_language( buf, hb_language_from_string( "ja-JP", -1 ) );
      hb_shape(font, buf, nullptr, 0);
      unsigned int glyph_count;
      hb_glyph_info_t *glyph_info = hb_buffer_get_glyph_infos( buf, &glyph_count );
      hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions( buf, &glyph_count );
      for (unsigned int gid = 0; gid < glyph_count; gid++) {
        hb_codepoint_t glyphid  = glyph_info[gid].codepoint;
        hb_position_t x_offset  = glyph_pos[gid].x_offset;
        hb_position_t y_offset  = glyph_pos[gid].y_offset;
        hb_position_t x_advance = glyph_pos[gid].x_advance;
        hb_position_t y_advance = glyph_pos[gid].y_advance;
        auto &temp = dest.glyphs;
        if( !r.is_rtl ){
          temp.insert(
            std::next( temp.begin(), ltr_cursor + gid ),
            gct::text::glyph{
              std::uint32_t( glyphid ),
              s.script_code,
              breakable && gid == 0,
              r.is_rtl,
              pxpem,
              x_offset,
              y_offset,
              x_advance,
              y_advance
            }
          );
        }
        else {
          temp.insert(
            std::next( temp.begin(), rtl_cursor + gid ),
            gct::text::glyph{
              std::uint32_t( glyphid ),
              s.script_code,
              breakable && gid == glyph_count - 1u,
              r.is_rtl,
              pxpem,
              x_offset,
              y_offset,
              x_advance,
              y_advance
            }
          );
        }
      }
      if( !r.is_rtl ) {
        ltr_cursor += glyph_count;
        rtl_cursor = ltr_cursor;
      }
      else {
        ltr_cursor += glyph_count;
      }
      hb_buffer_destroy( buf );
    }
  }
  return dest;
}

UChar32 get_hyphen( UChar32 v ) {
  UErrorCode error = U_ZERO_ERROR;
  const UScriptCode script_code = uscript_getScript( v, &error );
  if( U_FAILURE( error ) ) std::abort();

  if (script_code == USCRIPT_KANNADA ) {
    return UChar32( 0 ); // No hyphen
  }
  else if( script_code == USCRIPT_MALAYALAM ) {
    return UChar32( 0 ); // No hyphen
  }
  else if( script_code == USCRIPT_TAMIL ) {
    return UChar32( 0 ); // No hyphen
  }
  else if( script_code == USCRIPT_TELUGU ) {
    return UChar32( 0 ); // No hyphen
  }
  else if( script_code == USCRIPT_ARMENIAN ) {
    return UChar32( 0x058A ); // Armenian hyphen
  }
  else if( script_code == USCRIPT_CANADIAN_ABORIGINAL ) {
    return UChar32( 0x1400 ); // Canadian syllabics hyphen
  }
  else if( script_code == USCRIPT_HEBREW ) {
    return UChar32( 0x1400 ); // Hebrew punctuation maqaf
  }
  else if( script_code == USCRIPT_MONGOLIAN ) {
    return UChar32( 0x1806 ); // Mongolian todo soft hyphen
  }
  else if( script_code == USCRIPT_HIRAGANA ) {
    return UChar32( 0x0 ); // No hyphen
  }
  else if( script_code == USCRIPT_KATAKANA ) {
    return UChar32( 0x0 ); // No hyphen
  }
  else if( script_code == USCRIPT_KATAKANA_OR_HIRAGANA ) {
    return UChar32( 0x0 ); // No hyphen
  }
  else if( script_code == USCRIPT_JAPANESE ) {
    return UChar32( 0x0 ); // No hyphen
  }
  else if( script_code == USCRIPT_HANGUL ) {
    return UChar32( 0x0 ); // No hyphen
  }
  else if( script_code == USCRIPT_HAN ) {
    return UChar32( 0x0 ); // No hyphen
  }
  else {
    return UChar32( 0x2010 ); // Hyphen
  }
}

std::string to_utf8( const std::u32string &v ) {
  UErrorCode error = U_ZERO_ERROR;
  std::vector< UChar32 > utf32;
  utf32.reserve( v.size() );
  std::transform(
    v.begin(),
    v.end(),
    std::back_inserter( utf32 ),
    []( char32_t v ) -> UChar32 {
      return UChar32( v );
    }
  );
  std::vector< UChar > utf16( utf32.size() * 2u );
  std::int32_t utf16_length = 0u;
  u_strFromUTF32( utf16.data(), utf16.size(), &utf16_length, utf32.data(), utf32.size(), &error );
  if( U_FAILURE( error ) ) std::abort();
  utf16.resize( utf16_length );
  std::vector< char > utf8( utf32.size() * 4u );
  std::int32_t utf8_length = 0u;
  u_strToUTF8 ( utf8.data(), utf8.size(), &utf8_length, utf16.data(), utf16_length, &error );
  if( U_FAILURE( error ) ) std::abort();
  utf8.resize( utf8_length );
  return std::string( utf8.begin(), utf8.end() );
}


}

