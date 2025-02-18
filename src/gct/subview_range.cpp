#include <nlohmann/json.hpp>
#include <gct/subview_range.hpp>

namespace gct {
bool operator==( const subview_range &l, const subview_range &r ) {
  if( l.mip_offset != r.mip_offset ) return false; 
  if( l.mip_count != r.mip_count ) return false; 
  if( l.layer_offset != r.layer_offset ) return false; 
  if( l.layer_count != r.layer_count ) return false;
  if( l.force_array != r.force_array ) return false;
  return true;
}
bool operator!=( const subview_range &l, const subview_range &r ) {
  return !( l == r );
}
void to_json( nlohmann::json &dest, const subview_range &src ) {
  dest = nlohmann::json::object();
  dest[ "mip_offset" ] = src.mip_offset;
  dest[ "mip_count" ] = src.mip_count;
  dest[ "layer_offset" ] = src.layer_offset;
  dest[ "layer_count" ] = src.layer_count;
  dest[ "force_array" ] = src.force_array;
}
}

