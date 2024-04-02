#include <nlohmann/json.hpp>
#include <gct/buffer_window.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const buffer_window_t &src ) {
  dest = nlohmann::json::object();
  dest[ "index" ] = src.index;
  dest[ "offset" ] = src.offset;
}
void from_json( const nlohmann::json &src, buffer_window_t &dest ) {
  dest.index = src[ "index" ];
  dest.offset = src[ "offset" ];
}

}

