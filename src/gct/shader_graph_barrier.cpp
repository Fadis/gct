#include <nlohmann/json.hpp>
#include <gct/shader_graph_barrier.hpp>

namespace gct::shader_graph {
void to_json( nlohmann::json &dest, const barrier &src ) {
  dest = nlohmann::json::object();
  dest[ "state" ] = src.state;
  dest[ "view" ] = nlohmann::json::array();
  for( const auto &v: src.view ) {
    dest[ "view" ].push_back( *v );
  }
}
}

