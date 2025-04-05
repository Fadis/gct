#include <nlohmann/json.hpp>
#include <gct/shader_graph_subedge.hpp>

namespace gct::shader_graph {
void to_json( nlohmann::json &dest, const subedge &src ) {
  dest = nlohmann::json::object();
  dest[ "from" ] = src.from;
  dest[ "to" ] = src.to;
}
}

