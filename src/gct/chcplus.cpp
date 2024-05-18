#include <gct/chcplus.hpp>

namespace gct::scene_graph {

float chcplus_pkeep(
  unsigned int i
) {
  return 0.99f - 0.7 * std::exp( -float( i ) );
}

void chcplus_pfail(
  const std::vector< std::pair< aabb, resource_pair > > &group,
  std::vector< float > &dest,
  const visibility_history &vh
) {
  float n = 1.0f;
  for( const auto &v: group ) {
    const auto history = vh.get().find( *v.second.inst );
    n *= chcplus_pkeep( history->second.frame_count );
    dest.push_back( 1.0f - n );
  }
}

void chcplus_c(
  const std::vector< std::pair< aabb, resource_pair > > &group,
  std::vector< float > &dest,
  const visibility_history &vh
) {
  chcplus_pfail( group, dest, vh );
  for( unsigned int i = 0u; i != group.size(); ++i ) {
    dest[ i ] = 1 + dest[ i ] * ( i + 1u );
  }
}

void chcplus_v(
  const std::vector< std::pair< aabb, resource_pair > > &group,
  std::vector< float > &dest,
  const visibility_history &vh
) {
  chcplus_c( group, dest, vh );
  for( unsigned int i = 0u; i != group.size(); ++i ) {
    dest[ i ] = ( i + 1u )/dest[ i ];
  }
}

void sort_by_history(
  std::vector< std::pair< aabb, resource_pair > > &group,
  const visibility_history &vh
) {
  std::sort( group.begin(), group.end(),
    [&]( const auto &l, const auto &r ) {
      const auto lh = vh.get().find( *l.second.inst );
      const auto rh = vh.get().find( *r.second.inst );
      return lh->second.frame_count > rh->second.frame_count;
    }
  );
}

}

