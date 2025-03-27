#ifndef GCT_SHADER_GRAPH_BARRIER_HPP
#define GCT_SHADER_GRAPH_BARRIER_HPP

#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/barrier_config.hpp>
#include <gct/image_pool.hpp>

namespace gct {

struct shader_graph_barrier {
  LIBGCT_SETTER( state )
  LIBGCT_SETTER( view )
  barrier_state state;
  shader_graph_barrier &add_view( const image_pool::image_descriptor &v ) {
    view.push_back( v );
    return *this;
  }
  std::vector< image_pool::image_descriptor > view;
};

void to_json( nlohmann::json &dest, const shader_graph_barrier& );

}

#endif

