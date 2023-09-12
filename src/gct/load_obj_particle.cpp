#include <fstream>
#include <string>
#include <boost/spirit/include/qi.hpp>
#include <boost/fusion/container/vector.hpp>
#include <gct/load_obj_particle.hpp>

namespace gct {

std::tuple<
  std::vector< std::uint8_t >,
  std::uint32_t,
  aabb
>
load_obj_particle(
  const std::filesystem::path &filename,
  const std::unordered_map< vertex_attribute_usage_t, vertex_input_detail_t > &vamap,
  std::uint32_t stride
) {
  std::ifstream file( filename.string(), std::ios::in );
  if( !file.good() ) {
    throw -1;
  }
  std::vector< std::uint8_t > data;
  std::uint32_t count = 0u;
  glm::vec3 min;
  glm::vec3 max;
  while( !file.eof() ) {
    std::string line;
    std::getline( file, line );
    auto iter = line.begin();
    const auto end = std::find( line.begin(), line.end(), '#' );
    namespace qi = boost::spirit::qi;
    boost::fusion::vector< float, float, float > temp;
    const auto position = vamap.find( vertex_attribute_usage_t::POSITION );
    if( position == vamap.end() ) {
      throw -1;
    }
    std::cout << std::string( iter, end ) << std::endl;
    if( qi::parse(
      iter, end,
      qi::skip( qi::standard::space )[ qi::lit( 'v' ) >> qi::float_ >> qi::float_ >> qi::float_ ],
      temp
    ) /*&& iter == end*/ ) {
      const float x = boost::fusion::at_c< 0 >( temp );
      const float y = -boost::fusion::at_c< 1 >( temp );
      const float z = boost::fusion::at_c< 2 >( temp );
      min[ 0 ] = std::min( min[ 0 ], x );
      min[ 1 ] = std::min( min[ 1 ], y );
      min[ 2 ] = std::min( min[ 2 ], z );
      max[ 0 ] = std::max( min[ 0 ], x );
      max[ 1 ] = std::max( min[ 1 ], y );
      max[ 2 ] = std::max( min[ 2 ], z );
      data.resize( data.size() + stride, 0 );
      set_vertex_buffer_values( data.data(), count, position->second, stride, x, y, z, 1.f );
      ++count;
    }
  }
  return std::make_tuple(
    std::move( data ),
    count,
    aabb{
      min,
      max
    }
  );
}

}

