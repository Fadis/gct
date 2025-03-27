#include <nlohmann/json.hpp>
#include <gct/shader_graph_subedge.hpp>

namespace gct {
void to_json( nlohmann::json &dest, const shader_graph_subedge &src ) {
  dest = nlohmann::json::object();
  dest[ "from" ] = src.from;
  dest[ "to" ] = src.to;
}
}

