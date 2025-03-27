#ifndef GCT_SHADER_GRAPH_SUBEDGE_HPP
#define GCT_SHADER_GRAPH_SUBEDGE_HPP
#include <string>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct {
struct shader_graph_subedge {
  LIBGCT_SETTER( from )
  LIBGCT_SETTER( to )
  std::string from;
  std::string to;
};

using shader_graph_subedge_list = std::vector< shader_graph_subedge >;

void to_json( nlohmann::json &dest, const shader_graph_subedge &src );

}

#endif

