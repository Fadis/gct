#include <nlohmann/json.hpp>
#include <gct/graphics_execution_shape.hpp>
#include <gct/instance_list.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const graphics_execution_shape &src ) {
  dest = nlohmann::json::object();
  if( src.list ) {
    dest[ "list" ] = *src.list;
  }
  dest[ "offset" ] = src.offset;
  dest[ "count" ] = src.count;
  dest[ "x" ] = src.x;
  dest[ "y" ] = src.y;
  dest[ "z" ] = src.z;
}

void from_json( const nlohmann::json &src, graphics_execution_shape &dest ) {
  dest = nlohmann::json::object();
  if( src.find( "offset" ) != src.end() ) {
    dest.offset = src[ "offset" ];
  }
  if( src.find( "count" ) != src.end() ) {
    dest.count = src[ "count" ];
  }
  if( src.find( "x" ) != src.end() ) {
    dest.x = src[ "x" ];
  }
  if( src.find( "y" ) != src.end() ) {
    dest.y = src[ "y" ];
  }
  if( src.find( "z" ) != src.end() ) {
    dest.z = src[ "z" ];
  }
}

}


