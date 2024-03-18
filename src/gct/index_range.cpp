#include <nlohmann/json.hpp>
#include <gct/extension.hpp>
#include <gct/index_range.hpp>

namespace gct {
void to_josn( nlohmann::json &dest, const index_range &src ) {
  dest = nlohmann::json::object();
  dest[ "offset" ] = src.offset;
  dest[ "count" ] = src.count;
}

void from_josn( const nlohmann::json &src, index_range &dest ) {
  if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to index_range", __FILE__, __LINE__ );
  dest.offset = src[ "offset" ];
  dest.count = src[ "count" ];
}

}

