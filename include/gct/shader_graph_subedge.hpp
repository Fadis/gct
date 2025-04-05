#ifndef GCT_SHADER_GRAPH_SUBEDGE_HPP
#define GCT_SHADER_GRAPH_SUBEDGE_HPP
#include <string>
#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>

namespace gct::shader_graph {
struct subedge {
  LIBGCT_SETTER( from )
  LIBGCT_SETTER( to )
  std::string from;
  std::string to;
};

using subedge_list = std::vector< subedge >;

void to_json( nlohmann::json &dest, const subedge &src );

}

#endif

