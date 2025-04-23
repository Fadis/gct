#include <nlohmann/json.hpp>
#include <gct/type_id.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const type_id &src ) {
  if( src == type_id::float_ ) {
    dest = "float";
  }
  else if( src == type_id::u8 ) {
    dest = "u8";
  }
  else if( src == type_id::u16 ) {
    dest = "u16";
  }
  else if( src == type_id::u32 ) {
    dest = "u32";
  }
  else {
    dest = "unknown";
  }
}
}

