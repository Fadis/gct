#include <nlohmann/json.hpp>
#include <gct/shader_graph_barrier.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const shader_graph_barrier &src ) {
  dest = nlohmann::json::object();
  dest[ "state" ] = src.state;
  dest[ "view" ] = nlohmann::json::array();
  for( const auto &v: src.view ) {
    dest[ "view" ].push_back( *v );
  }
}
}

