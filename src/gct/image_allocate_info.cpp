#include <nlohmann/json.hpp>
#include <gct/image_allocate_info.hpp>

namespace gct {

bool operator==( const image_allocate_info &l, const image_allocate_info &r ) {
  if( l.create_info != r.create_info ) return false;
  if( l.range != r.range ) return false;
  if( l.layout != r.layout ) return false;
  return true;
}

bool operator!=( const image_allocate_info &l, const image_allocate_info &r ) {
  return !( l == r );
}

void to_json( nlohmann::json &dest, const image_allocate_info &src ) {
  dest = nlohmann::json::object();
  dest[ "create_info" ] = src.create_info;
  if( src.range ) {
    dest[ "range" ] = *src.range;
  }
  if( src.layout ) {
    dest[ "layout" ] = *src.layout;
  }
}


}

