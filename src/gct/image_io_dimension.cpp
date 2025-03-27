#include <nlohmann/json.hpp>
#include <gct/image_io_dimension.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const image_io_dimension &src ) {
  dest = nlohmann::json::object();
  if( src.relative_to ) {
    dest[ "relative_to" ] = *src.relative_to;
  }
  dest[ "size_transform" ] = nlohmann::json::array();
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 0 ][ 3 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 1 ][ 3 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 2 ][ 3 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 0 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 1 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 2 ] );
  dest[ "size_transform" ].push_back( src.size_transform[ 3 ][ 3 ] );
}
}

