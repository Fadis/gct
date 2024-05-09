#include <gct/chc.hpp>

namespace gct::scene_graph {

void build_visible_set(
  const std::vector< resource_pair > &src,
  std::unordered_set< unsigned int > &dest
) {
  dest.clear();
  for( const auto &v: src ) {
    dest.insert( *v.inst );
  }
}

}

