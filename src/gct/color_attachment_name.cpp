#include <nlohmann/json.hpp>
#include <gct/color_attachment_name.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const color_attachment_name &src ) {
  dest = nlohmann::json::object();
  dest[ "location" ] = src.location;
  dest[ "name" ] = src.name;
  dest[ "type" ] = src.type;
}

void from_json( const nlohmann::json &src, color_attachment_name &dest ) {
  dest = nlohmann::json::object();
  if( src.find( "location" ) != src.end() ) {
    dest.location = src[ "location" ];
  }
  if( src.find( "name" ) != src.end() ) {
    dest.name = src[ "name" ];
  }
  if( src.find( "type" ) != src.end() ) {
    dest.type = src[ "type" ];
  }
}

}


