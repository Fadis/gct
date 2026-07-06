#include <nlohmann/json.hpp>
#include <gct/meshlet_type.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const meshlet_type &src ) {
  dest = nlohmann::json::object();
  dest[ "center" ] = nlohmann::json::array();
  dest[ "center" ].push_back( src.center.x );
  dest[ "center" ].push_back( src.center.y );
  dest[ "center" ].push_back( src.center.z );
  dest[ "center" ].push_back( src.center.w );
  dest[ "normal" ] = nlohmann::json::array();
  dest[ "normal" ].push_back( src.normal.x );
  dest[ "normal" ].push_back( src.normal.y );
  dest[ "normal" ].push_back( src.normal.z );
  dest[ "normal" ].push_back( src.normal.w );

}
void from_json( const nlohmann::json &src, meshlet_type &dest ) {
  if( src.is_object() ) {
    if( src.find( "center" ) != src.end() ) {
      if( src[ "center" ].is_array() ) {
        dest.set_center(
          glm::vec4(
            ( src[ "center" ].size() > 0 ) ? float( src[ "center" ][ 0 ] ) : 0.f,
            ( src[ "center" ].size() > 1 ) ? float( src[ "center" ][ 1 ] ) : 0.f,
            ( src[ "center" ].size() > 2 ) ? float( src[ "center" ][ 2 ] ) : 0.f,
            ( src[ "center" ].size() > 3 ) ? float( src[ "center" ][ 3 ] ) : 0.f
          )
        );
      }
    }
    if( src.find( "normal" ) != src.end() ) {
      if( src[ "normal" ].is_array() ) {
        dest.set_normal(
          glm::vec4(
            ( src[ "normal" ].size() > 0 ) ? float( src[ "normal" ][ 0 ] ) : 0.f,
            ( src[ "normal" ].size() > 1 ) ? float( src[ "normal" ][ 1 ] ) : 0.f,
            ( src[ "normal" ].size() > 2 ) ? float( src[ "normal" ][ 2 ] ) : 0.f,
            ( src[ "normal" ].size() > 3 ) ? float( src[ "normal" ][ 3 ] ) : 0.f
          )
        );
      }
    }
  }
}

}

