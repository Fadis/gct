#include <nlohmann/json.hpp>
#include <gct/key_value.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const key_value_t &src ) {
  dest = nlohmann::json::object();
  dest[ "key" ] = src.key;
  dest[ "value" ] = src.value;
}
void from_json( const nlohmann::json &src, key_value_t &dest ) {
  dest = key_value_t();
  if( src.find( "key" ) != src.end() ) {
    dest.key = src[ "key" ];
  }
  if( src.find( "value" ) != src.end() ) {
    dest.value = src[ "value" ];
  }
}

}

