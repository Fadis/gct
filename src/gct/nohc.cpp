#include <gct/nohc.hpp>

namespace gct::scene_graph {

float nohc_next_cscr(
  float current_cscr,
  float current_c
) {
  return current_cscr + ( 1.f - current_cscr ) * current_c;
}
float nohc_pcov(
  float cscr,
  float cbb
) {
  if( cbb < cscr ) {
    const float x = std::sqrt( cscr ) - std::sqrt( cbb );
    return x * x;
  }
  else {
    return 0.f;
  }
}

unsigned int get_vertex_count(
  const std::vector< resource_pair > &v,
  const instance_list &il
) {
  unsigned int sum = 0u;
  for( const auto &e: v ) {
    const auto prim = il.get_resource()->prim.get( e.prim );
    sum += prim->count;
  }
  return sum;
}

}

