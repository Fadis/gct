#ifndef GCT_SHADER_GRAPH_BARRIER_HPP
#define GCT_SHADER_GRAPH_BARRIER_HPP

#include <vector>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/barrier_config.hpp>
#include <gct/image_pool.hpp>

namespace gct::shader_graph {

struct barrier {
  LIBGCT_SETTER( state )
  LIBGCT_SETTER( view )
  barrier_state state;
  barrier &add_view( const image_pool::image_descriptor &v ) {
    view.push_back( v );
    return *this;
  }
  std::vector< image_pool::image_descriptor > view;
};

void to_json( nlohmann::json &dest, const barrier& );

}

#endif

