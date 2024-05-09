#ifndef GCT_CHC_HPP
#define GCT_CHC_HPP
#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/instance_list.hpp>
#include <gct/query_pool.hpp>
#include <gct/aabb.hpp>
#include <gct/kdtree.hpp>
#include <gct/instance_list.hpp>

namespace gct::scene_graph {

struct chc_node_data {
  bool visible = false;
};

template< typename T >
void
get_visibility_unknown_node(
  const std::vector< T > &src,
  std::vector< T > &dest,
  std::vector< gct::scene_graph::resource_pair > &visible
) {
  std::vector< T > candidate = src;
  std::vector< T > temp;
  std::vector< T > unknown;
  while( !candidate.empty() ) {
    for( const auto &n: candidate ) {
      if( !n.empty() ) {
        if( n.get_root_node().value.visible ) {
          if( n.get_root_node().leaf ) {
            n.get( visible );
          }
          else {
            const auto [lt,ge] = n.subtree();
            if( !lt.empty() ) {
              temp.push_back( lt );
            }
            if( !ge.empty() ) {
              temp.push_back( ge );
            }
          }
        }
        else {
          unknown.push_back( n );
        }
      }
    }
    candidate = std::move( temp );
    temp.clear();
  }
  dest = std::move( unknown );
}

template< typename T, typename U >
void
chc(
  const std::vector< T > &candidate,
  std::vector< T > &not_occluded,
  std::vector< gct::scene_graph::resource_pair > &visible,
  U &occlusion_query,
  glm::vec3 eye_pos,
  float znear
) {
  const auto near_box = gct::create_cube_area( eye_pos, znear*2 );
  occlusion_query.reset();
  for( const auto &c: candidate ) {
    if( !c.empty() ) {
      const auto aabb = c.get_aabb();
      if( aabb && near_box ) {
        if( c.is_leaf() ) {
          c.get( visible );
        }
        else {
          const auto [lt,ge] = c.subtree();
          if( !lt.empty() ) {
            not_occluded.push_back( lt );
          }
          if( !ge.empty() ) {
            not_occluded.push_back( ge );
          }
        }
      }
      else {
        if( c.is_leaf() ) {
          c.get( visible );
        }
        else {
          const auto [lt,ge] = c.subtree();
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


template< typename T >
void mark_node_visibility(
  T &visible_tree,
  const std::unordered_set< unsigned int > &visible_set
) {
  using node_type = std::shared_ptr< typename T::node_type >;
  visible_tree.for_each_node(
    [&]( const node_type &node ) {
      if( node->leaf ) {
        node->value.visible = visible_set.find( *node->leaf->value[ 0 ].inst ) != visible_set.end();
      }
      else if( node->less_than && node->less_than ) {
        const auto lt = node->less_than->value.visible;
        const auto ge = node->less_than->value.visible;
        node->value.visible = lt || ge;
      }
      else if( node->less_than ) {
        const auto lt = node->less_than->value.visible;
        node->value.visible = lt;
      }
      else if( node->greater_equal ) {
        const auto ge = node->greater_equal->value.visible;
        node->value.visible = ge;
      }
      else {
        node->value.visible = false;
      }
    }
  );
}

void build_visible_set(
  const std::vector< resource_pair > &src,
  std::unordered_set< unsigned int > &dest
);

}
#endif


