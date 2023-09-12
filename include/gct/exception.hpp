#ifndef LIBGCT_EXCEPTION_HPP
#define LIBGCT_EXCEPTION_HPP
/*
 * Copyright (C) 2020 Naomasa Matsubayashi
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
#include <stdexcept>
#define LIBGCT_GENERATE_EXCEPTION( base ) \
  class base : public std:: base { \
  public: \
    base ( const std::string &what ) : std:: base ( what ), where_( nullptr ), line_( 0 ) {} \
    base ( const char *what ) : std:: base ( what ), where_( nullptr ), line_( 0 ) {} \
    base ( const char *what, const char *w, int l ) : std:: base ( what ), where_( w ), line_( l ) {} \
    base ( const std::string &what, const char *w, int l ) : std:: base ( what ), where_( w ), line_( l ) {} \
    const char *where() const { return where_; } \
    int line() const { return line_; } \
  private: \
    const char *where_; \
    int line_; \
  };
namespace gct::exception {
  LIBGCT_GENERATE_EXCEPTION( logic_error )
  LIBGCT_GENERATE_EXCEPTION( domain_error )
  LIBGCT_GENERATE_EXCEPTION( invalid_argument )
  LIBGCT_GENERATE_EXCEPTION( length_error )
  LIBGCT_GENERATE_EXCEPTION( out_of_range )
  LIBGCT_GENERATE_EXCEPTION( runtime_error )
  LIBGCT_GENERATE_EXCEPTION( range_error )
  LIBGCT_GENERATE_EXCEPTION( overflow_error )
  LIBGCT_GENERATE_EXCEPTION( underflow_error )
}
#undef LIBGCT_GENERATE_EXCEPTION
#define LIBGCT_EXCEPTION( base, name, description ) \
  struct name : public ::gct::exception:: base { \
    name () : ::gct::exception:: base ( description ) {} \
    name ( const std::string &what ) : ::gct::exception:: base ( what ) {} \
    name ( const char *what ) : ::gct::exception:: base ( what ) {} \
    name ( const char *what, const char *w, int l ) : ::gct::exception:: base ( what, w, l ) {} \
    name ( const std::string &what, const char *w, int l ) : ::gct::exception:: base ( what, w, l ) {} \
  };
#endif

