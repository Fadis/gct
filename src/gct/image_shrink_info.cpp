#include <nlohmann/json.hpp>
#include <gct/image_shrink_info.hpp>
namespace gct {

void to_json( nlohmann::json &root, const image_shrink_info &v ) {
  root = nlohmann::json::object();
  root[ "extent" ] = nlohmann::json::array();
  root[ "extent" ].push_back( v.extent[ 0 ] );
  root[ "extent" ].push_back( v.extent[ 1 ] );
  root[ "extent" ].push_back( v.extent[ 2 ] );
}
void from_json( const nlohmann::json &root, image_shrink_info &v ) {
  v = image_shrink_info();
  if( root.find( "extent" ) != root.end() ) {
    if( root[ "extent" ].size() >= 1u ) {
      v.extent[ 0 ] = root[ "extent" ][ 0 ];
    }
    if( root[ "extent" ].size() >= 2u ) {
      v.extent[ 1 ] = root[ "extent" ][ 1 ];
    }
    if( root[ "extent" ].size() >= 3u ) {
      v.extent[ 2 ] = root[ "extent" ][ 2 ];
    }
  }
}

}

