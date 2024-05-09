#ifndef GCT_NOHC_HPP
#define GCT_NOHC_HPP
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

struct nohc_node_data {
  bool visible = false;
  float poccl = 0.0f;
};

float nohc_next_cscr(
  float current_cscr,
  float current_c
);
float nohc_pcov(
  float cscr,
  float cbb
);
unsigned int get_vertex_count(
  const std::vector< resource_pair > &v,
  const instance_list &il
);

template< typename T >
float nohc_cbb(
  const T &oi,
  const glm::vec3 &eye_pos,
  std::uint32_t width,
  std::uint32_t height,
  float fovy
) {
  const auto d = glm::distance( get_center( oi.get_aabb() ), eye_pos );
  const auto x = ( d * std::sqrt( float( width )/float( height ) ) * 2.0f* std::tan( fovy/2.0f ) );
  const auto xm2 = ( 1.0f/x )*( 1.0f/x );
  return xm2*surface_area( oi.get_aabb() )/6.0f;
}

template< typename T >
float nohc_rcov(
  const T &oi
) {
  const float r = get_inner_sphere_radius( oi.get_aabb() );
  const float a = 4.f * M_PI * r * r;
  return 3.f / 2.f * a / surface_area( oi.get_aabb() );
}

template< typename T >
float nohc_c(
  const T &oi,
  float current_cbb
) {
  return nohc_rcov( oi ) * current_cbb;
}

template< typename T >
unsigned int get_vertex_count(
  const T &v,
  const instance_list &il
) {
  std::vector< resource_pair > temp;
  v.get( temp );
  return get_vertex_count( temp, il );
}


template< typename T >
float nohc_tr(
  const T &oi,
  const instance_list &il,
  const nohc_parameter &p,
  const glm::vec3 &eye_pos,
  std::uint32_t width,
  std::uint32_t height,
  float fovy
) {
  return std::max(
    std::max(
      p.trs,
      p.trv * get_vertex_count( oi, il )
    ),
    p.trf * nohc_cbb( oi, eye_pos, width, height, fovy ) 
  );
}

template< typename T >
float nohc_to(
  const T &oi,
  const instance_list &il,
  const nohc_parameter &p,
  const glm::vec3 &eye_pos,
  std::uint32_t width,
  std::uint32_t height,
  float fovy
) {
  return std::max(
    p.tos,
    p.top * nohc_cbb( oi, eye_pos, width, height, fovy ) 
  );
}

template< typename T >
float nohc_b(
  const T &oi,
  const instance_list &il,
  const nohc_parameter &p,
  const glm::vec3 &eye_pos,
  std::uint32_t width,
  std::uint32_t height,
  float fovy,
  unsigned int n
) {
  const float poccl = oi.get_root_node().value.poccl;
  const float tr = nohc_tr( oi, il, p, eye_pos, width, height, fovy );
  const float to = nohc_to( oi, il, p, eye_pos, width, height, fovy );
  const float l = std::log2( n + 1 );
  return poccl * ( tr - to )/l;
}

template< typename T >
bool nohc(
  const T &oi,
  const instance_list &il,
  const nohc_parameter &p,
  const glm::vec3 &eye_pos,
  std::uint32_t width,
  std::uint32_t height,
  float fovy,
  unsigned int n
) {
  float c = nohc_to( oi, il, p, eye_pos, width, height, fovy );
  float b = nohc_b( oi, il, p, eye_pos, width, height, fovy, n );
  return c <= b;
}

template< typename T >
float nohc_next_cscr(
  float current_cscr,
  const T &oi,
  const glm::vec3 &eye_pos,
  std::uint32_t width,
  std::uint32_t height,
  float fovy
) {
  return nohc_next_cscr( current_cscr, nohc_c( oi, nohc_cbb( oi, eye_pos, width, height, fovy ) ) );
}


template< typename T >
void build_invisible_set(
  const T &visible_tree,
  const std::unordered_set< unsigned int > &visible_set,
  std::unordered_set< unsigned int > &dest
) {
  dest.clear();
  std::vector< resource_pair > visible;
  visible_tree.get( visible );
  for( const auto &v: visible ) {
    dest.insert( *v.inst );
  }
  for( const auto &v: visible_set ) {
    dest.erase( v );
  }
}


template< typename T >
void calculate_poccl(
  T &visible_tree,
  const std::unordered_set< unsigned int > &visible_set,
  const std::unordered_set< unsigned int > &invisible_set,
  const glm::vec3 &eye_pos,
  unsigned int width,
  unsigned int height,
  float fovy
) {
  std::vector< gct::kdtree< gct::scene_graph::resource_pair, gct::scene_graph::nohc_node_data > > leaf;
  visible_tree.get_leaf_node( leaf );
  gct::scene_graph::sort_by_distance(
    leaf,
    eye_pos
  );
  float cscr = 0.f;
  for( auto &l: leaf ) {
    const auto inst = *l.get_root_node().leaf->value[ 0 ].inst;
    l.get_root_node().value.poccl = gct::scene_graph::nohc_pcov(
      cscr,
      gct::scene_graph::nohc_cbb(
        l, eye_pos, width, height, fovy
      )
    );
    cscr = gct::scene_graph::nohc_next_cscr(
      cscr, l, eye_pos, width, height, fovy
    );
    if( visible_set.find( inst ) != visible_set.end() ) {
      l.get_root_node().value.poccl = 0.5f * l.get_root_node().value.poccl * l.get_root_node().value.poccl;
    }
    else if( invisible_set.find( inst ) != invisible_set.end() ) {
      l.get_root_node().value.poccl = 1.0f;
    }
  }
  const unsigned int n = visible_tree.size();
  visible_tree.for_each_node(
    [&]( auto &n ) {
      if( !n->leaf ) {
        bool lt = n->less_than ? n->less_than->value.poccl : 1.0f;
        bool ge = n->greater_equal ? n->greater_equal->value.poccl : 1.0f;
        n->value.poccl = std::min( lt, ge );
      }
    }
  );
}


template< typename T, typename U >
void nohc_(
  std::vector< T > &candidate,
  const instance_list &view_il,
  const nohc_parameter &nohc_param,
  std::vector< T > &not_occluded,
  std::vector< gct::scene_graph::resource_pair > &visible,
  U &occlusion_query,
  const glm::vec3 &eye_pos,
  unsigned int width,
  unsigned int height,
  float fovy,
  float znear,
  unsigned int n
) {
  const auto near_box = gct::create_cube_area( eye_pos, znear*2 );
  occlusion_query.reset();
  std::vector< T > temp{};
  for( const auto &v: candidate ) {
    if( gct::scene_graph::nohc(
      v,
      view_il,
      nohc_param,
      eye_pos,
      width,
      height,
      fovy,
      n
    ) ) {
      const auto aabb = v.get_aabb();
      if( aabb && near_box ) {
        v.get( visible );
      }
      else {
        if( v.get_root_node().leaf ) {
          v.get( visible );
        }
        else {
          const auto [lt,ge] = v.subtree();
          if( !lt.empty() ) {
            occlusion_query.push( lt.get_aabb(), lt );
          }
          if( !ge.empty() ) {
            occlusion_query.push( ge.get_aabb(), ge );
          }
        }
      }
    }
    else {
      if( v.get_root_node().leaf ) {
        v.get( visible );
        /*const auto aabb = v.get_aabb();
        if( aabb && near_box ) {
          v.get( visible );
        }
        else {
          v.get( visible );
        }*/
      }
      else {
        const auto [lt,ge] = v.subtree();
        if( !lt.empty() ) {
          temp.push_back( lt );
        }
        if( !ge.empty() ) {
          temp.push_back( ge );
        }
      }
    }
  }
  not_occluded = std::move( temp );
}

}

#endif

