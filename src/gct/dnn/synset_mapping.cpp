#include <algorithm>
#include <gct/mmaped_file.hpp>
#include <gct/dnn/synset_mapping.hpp>

namespace gct::dnn {

std::vector< std::string > load_synset_mapping(
  const std::filesystem::path &filename
) {
  const auto temp = mmaped_file( filename );
  auto iter = temp.begin();
  const auto end = temp.end();
  std::vector< std::string > dest;
  while( iter != end ) {
    iter = std::find_if( iter, end, []( const auto &v ) { return v > ' ' && v <= '~'; } );
    if( iter == end ) break;
    iter = std::find( iter, end, ' ' );
    if( iter == end ) break;
    iter = std::find_if( iter, end, []( const auto &v ) { return v > ' ' && v <= '~'; } );
    if( iter == end ) break;
    const auto eol = std::find_if( iter, end, []( const auto &v ) { return v == '\r' || v == '\n'; } );
    dest.emplace_back( iter, eol );
    iter = eol;
  }
  return dest;
}

}

