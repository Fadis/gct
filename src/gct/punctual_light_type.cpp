#include <nlohmann/json.hpp>
#include <gct/exception.hpp>
#include <gct/punctual_light_type.hpp>
namespace gct {
void to_json( nlohmann::json &dest, const punctual_light_type &src ) {
  if( src == punctual_light_type::directional ) {
    dest = "directional";
  }
  else if( src == punctual_light_type::point ) {
    dest = "point";
  }
  else if( src == punctual_light_type::spot ) {
    dest = "spot";
  }
  else {
    dest = "unknown"; 
  }
}

void from_json( const nlohmann::json &src, punctual_light_type &dest ) {
  if( src == "directional" ) {
    dest = punctual_light_type::directional;
  }
  else if( src == "point" ) {
    dest = punctual_light_type::point;
  }
  else if( src == "spot" ) {
    dest = punctual_light_type::spot;
  }
  else {
    throw exception::runtime_error( "incompatible punctual_light_type value", __FILE__, __LINE__ );
  }
}

}

