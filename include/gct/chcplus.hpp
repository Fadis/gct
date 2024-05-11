#ifndef GCT_CHCPLUS_HPP
#define GCT_CHCPLUS_HPP
#include <iostream>
#include <cmath>
#include <nlohmann/json.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/instance_list.hpp>
#include <gct/query_pool.hpp>
#include <gct/aabb.hpp>
#include <gct/kdtree.hpp>
#include <gct/instance_list.hpp>
#include <gct/visibility_history.hpp>

namespace gct::scene_graph {

float chcplus_pkeep(
  unsigned int i
);
void chcplus_pfail(
  const std::vector< std::pair< aabb, resource_pair > > &group,
  std::vector< float > &dest,
  const visibility_history &vh
);
void chcplus_c(
  const std::vector< std::pair< aabb, resource_pair > > &group,
  std::vector< float > &dest,
  const visibility_history &vh
);
void chcplus_v(
  const std::vector< std::pair< aabb, resource_pair > > &group,
  std::vector< float > &dest,
  const visibility_history &vh
);
void sort_by_history(
  std::vector<  std::pair< aabb, resource_pair > > &group,
  const visibility_history &vh
);

template< typename T >
void chcplus_split(
  T &group_tree,
  T &concistent_tree,
  T &inconcistent_tree,
  const visibility_history &vh
) {
  std::vector< std::pair< aabb, resource_pair > > group;
  group_tree.for_each_leaf_node(
    [&]( const auto &n ) {
      for( const auto &v: n->leaf->value ) {
        const auto history = vh.get().find( *v.inst );
        auto temp = v;
        temp.visible = history->second.state == visibility_state::visible;
        temp.frame_count = history->second.frame_count;
        group.push_back( std::make_pair( n->range, temp ) );
      }
    }
  );
  sort_by_history( group, vh );
  std::vector< float > v;
  chcplus_v( group, v, vh );
  const auto max = std::distance( v.begin(), std::max_element( v.begin(), v.end() ) ) + 1u;
  for( unsigned int i = 0u; i != max; ++i ) {
    concistent_tree.insert( group[ i ].first, group[ i ].second );
  }
  for( unsigned int i = max; i != group.size(); ++i ) {
    inconcistent_tree.insert( group[ i ].first, group[ i ].second );
  }
}

template< typename T, typename U >
void
chcplus(
  std::vector< T > &candidate,
  std::vector< T > &not_occluded,
  std::vector< gct::scene_graph::resource_pair > &visible,
  U &occlusion_query,
  const visibility_history &vh,
  glm::vec3 eye_pos,
  float znear
) {
  const auto near_box = gct::create_cube_area( eye_pos, znear*2 );
  occlusion_query.reset();
  for( auto &c: candidate ) {
    if( c.empty() ) {
    }
    else if( c.is_leaf() ) {
      c.get( visible );
    }
    else {
      T concistent;
      T inconcistent;
      chcplus_split( c, concistent, inconcistent, vh );
      std::vector< aabb4 > concistent_aabb;
      if( !concistent.empty() ) {
        bool close = false;
        concistent.for_each_leaf_node(
          [&]( const auto &n ) {
            if( aabb4( n->range ) && near_box ) {
              close = true;
            }
            concistent_aabb.push_back( n->range );
          }
        );
        if( close ) {
          const auto [lt,ge] = concistent.subtree();
          if( !lt.empty() ) {
            not_occluded.push_back( lt );
          }
          if( !ge.empty() ) {
            not_occluded.push_back( ge );
          }
        }
        else {
          if( inconcistent.empty() ) {
            const auto [lt,ge] = concistent.subtree();
            if( !lt.empty() ) {
              if( lt.get_aabb() && near_box ) {
                not_occluded.push_back( lt );
              }
              else {
                occlusion_query.push( lt.get_aabb(), lt );
              }
            }
            if( !ge.empty() ) {
              if( ge.get_aabb() && near_box ) {
                not_occluded.push_back( ge );
              }
              else {
                occlusion_query.push( ge.get_aabb(), ge );
              }
            }
          }
          else {
            occlusion_query.push( concistent_aabb, concistent );
          }
        }
      }
      if( !inconcistent.empty() ) {
        const auto aabb = inconcistent.get_aabb();
        if( aabb && near_box ) {
          const auto [lt,ge] = inconcistent.subtree();
          if( !lt.empty() ) {
            not_occluded.push_back( lt );
          }
          if( !ge.empty() ) {
            not_occluded.push_back( ge );
          }
        }
        else {
          const auto [lt,ge] = inconcistent.subtree();
          if( !lt.empty() ) {
            occlusion_query.push( lt.get_aabb(), lt );
          }
          if( !ge.empty() ) {
            occlusion_query.push( ge.get_aabb(), ge );
          }
        }
      }
    }
  }
}

}

#endif

