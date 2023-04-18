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
#include <boost/spirit/include/qi.hpp>
#include <boost/phoenix.hpp>
#include <gct/shader_flag.hpp>

namespace gct {
  template< typename Iterator >
  class shader_flag : public boost::spirit::qi::grammar< Iterator, shader_flag_t > {
  public:
    shader_flag() : shader_flag::base_type( root ) {
      namespace qi = boost::spirit::qi;
      namespace phx = boost::phoenix;
      keywords.add
        ( "bc", shader_flag_t::base_color )
        ( "mr", shader_flag_t::metallic_roughness )
        ( "no", shader_flag_t::normal )
        ( "oc", shader_flag_t::occlusion )
        ( "em", shader_flag_t::emissive )
        ( "sh", shader_flag_t::shadow )
        ( "tangent", shader_flag_t::tangent )
        ( "world", shader_flag_t( 0 ) );
      targets.add
        ( "vert", shader_flag_t::vertex )
        ( "tesc", shader_flag_t::tesselation_control )
        ( "tese", shader_flag_t::tesselation_evaluation )
        ( "geom", shader_flag_t::geometry )
        ( "frag", shader_flag_t::fragment )
        ( "comp", shader_flag_t::compute );
      normal = ( ( keywords % '_' ) >> '.' >> targets >> ".spv" )[
        qi::_pass = phx::bind( &shader_flag::combine, qi::_val, qi::_1, qi::_2 )
      ];
      special = ( "special" >> qi::uint_ >> '.' >> targets >> ".spv" )[
        qi::_pass = phx::bind( &shader_flag::combine_special, qi::_val, qi::_1, qi::_2 )
      ];
      root = normal | special;
    }
  private:
    static bool combine( shader_flag_t &dest, const std::vector< shader_flag_t > &keywords, shader_flag_t target ) {
      int v = int( target );
      for( const auto &k: keywords ) v |= int( k );
      dest = shader_flag_t( v );
      return true;
    }
    static bool combine_special( shader_flag_t &dest, unsigned int n, shader_flag_t target ) {
      int v = int( shader_flag_t::special )|int( target )|int( n & 0x0F );
      dest = shader_flag_t( v );
      return true;
    }
    boost::spirit::qi::symbols< char, shader_flag_t > keywords;
    boost::spirit::qi::symbols< char, shader_flag_t > targets;
    boost::spirit::qi::rule< Iterator, shader_flag_t > normal;
    boost::spirit::qi::rule< Iterator, shader_flag_t > special;
    boost::spirit::qi::rule< Iterator, shader_flag_t > root;
  };
  std::optional< shader_flag_t > get_shader_flag( const std::filesystem::path &path ) {
    auto filename = path.filename().string();
    auto begin = filename.cbegin();
    auto end = filename.cend();
    static const shader_flag< std::string::const_iterator > rule;
    auto flag = shader_flag_t( 0 );
    if( boost::spirit::qi::parse( begin, end, rule, flag ) ) return flag;
    return std::nullopt;
  }
}

