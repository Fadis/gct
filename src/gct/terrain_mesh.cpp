#include <iostream>
#include <unordered_map>
#include <boost/graph/breadth_first_search.hpp>
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <glm/geometric.hpp>
#include <gct/gaussian_blue_noise.hpp>
#include <gct/terrain_mesh.hpp>
#include <gct/area.hpp>
#include <gct/extension.hpp>
#include <gct/linear_interpolation.hpp>
#include <gct/cross2.hpp>
#include <gct/jc_voronoi.h>

namespace gct {


void to_json( nlohmann::json &dest, const terrain_vertex &src ) {
  dest = nlohmann::json::object();
  dest[ "position" ] = nlohmann::json::array();
  dest[ "position" ].push_back( src.position.x );
  dest[ "position" ].push_back( src.position.y );
  dest[ "position" ].push_back( src.position.z );
  dest[ "area" ] = src.area;
  dest[ "border" ] = src.border;
}

void from_json( const nlohmann::json &src, terrain_vertex &dest ) {
  if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to terrain_vertex", __FILE__, __LINE__ );
  dest = terrain_vertex{};
  if( src.find( "position" ) != src.end() ) {
    if( !src[ "position" ].is_array() ) throw incompatible_json( "The JSON is incompatible to terrain_vertex", __FILE__, __LINE__ );
    if( src[ "position" ].size() != 3u ) throw incompatible_json( "The JSON is incompatible to terrain_vertex", __FILE__, __LINE__ );
    unsigned int i = 0u;
    for( const auto &v: src[ "position" ] ) {
      if( !v.is_number() ) throw incompatible_json( "The JSON is incompatible to terrain_vertex", __FILE__, __LINE__ );
      dest.position[ i ] = float( v );
      ++i;
    }
  }
  if( src.find( "area" ) != src.end() ) {
    if( !src[ "area" ].is_number() ) throw incompatible_json( "The JSON is incompatible to terrain_vertex", __FILE__, __LINE__ );
    dest.area = float( src[ "area" ] );
  }
  if( src.find( "border" ) != src.end() ) {
    dest.border = bool( src[ "border" ] );
  }
}

glm::ivec2 sort_element( const glm::ivec2 v ) {
  auto temp = v;
  if( temp.x > temp.y ) std::swap( temp.x, temp.y );
  return temp;
}

glm::ivec3 sort_element( const glm::ivec3 v ) {
  auto temp = v;
  if( temp.x > temp.y ) std::swap( temp.x, temp.y );
  if( temp.y > temp.z ) std::swap( temp.y, temp.z );
  if( temp.x > temp.y ) std::swap( temp.x, temp.y );
  return temp;
}

terrain_mesh::terrain_mesh( const terrain_mesh_create_info &create_info ) :
  property( create_info ) {
  auto result = gct::gaussian_blue_noise( props.point_count, 1000 );
  std::vector< jcv_point > points;
  for( auto &v: result ) {
    jcv_point p;
    p.x = v.x * props.distance_scale.x;
    p.y = v.y * props.distance_scale.y;
    points.push_back( p );
  }
  jcv_rect bounding_box = { { 0.0f, 0.0f }, { 1.0f * props.distance_scale.x, 1.0f * props.distance_scale.y } };
  jcv_diagram diagram;
  memset(&diagram, 0, sizeof(jcv_diagram));
  jcv_diagram_generate( points.size(), points.data(), &bounding_box, 0, &diagram );

  const auto sites = jcv_diagram_get_sites( &diagram );
  vertex.resize( diagram.numsites );
  for( std::size_t i = 0u; i < diagram.numsites; ++i ) {
    vertex[ sites[ i ].index ].set_position( glm::vec3{ sites[ i ].p.x, sites[ i ].p.y, 0.0f } );
    
    jcv_edge_iter edge_iter;
    jcv_edge graph_edge;
    jcv_site_get_edges(&diagram, &sites[i], &edge_iter);
    std::vector< glm::vec2 > voronoi_vertex;
    while (jcv_edge_next(&edge_iter, &graph_edge)) {
      voronoi_vertex.push_back( glm::vec2( graph_edge.pos[ 0 ].x, graph_edge.pos[ 0 ].y ) );
    }
    vertex[ sites[ i ].index ].set_uplift(
      linear_interpolation( props.uplift, 0u, glm::vec2( vertex[ sites[ i ].index ].position ) / props.distance_scale ) * props.max_uplift
    );
    vertex[ sites[ i ].index ].set_area( gct::area( voronoi_vertex ) );
  }

  std::unordered_map< glm::ivec3, std::size_t > known_triangle;

  jcv_delaunay_iter delaunay;
  jcv_delaunay_edge delaunay_edge;

  std::unordered_multimap< std::size_t, std::size_t > edge_connection;

  jcv_delaunay_begin( &diagram, &delaunay );
  while( jcv_delaunay_next( &delaunay, &delaunay_edge ) ) {
    const auto e = glm::ivec2{
      delaunay_edge.sites[ 0 ]->index,
      delaunay_edge.sites[ 1 ]->index
    };
    edge.push_back( e );
    edge_connection.insert( edge_connection.end(), std::make_pair( e.x, e.y ) );
    edge_connection.insert( edge_connection.end(), std::make_pair( e.y, e.x ) );
  }

  jcv_delaunay_begin( &diagram, &delaunay );
  while( jcv_delaunay_next( &delaunay, &delaunay_edge ) ) {
    const auto e = glm::ivec2{
      delaunay_edge.sites[ 0 ]->index, // a
      delaunay_edge.sites[ 1 ]->index // b
    };
    const auto [c_begin, c_end] = edge_connection.equal_range( delaunay_edge.sites[ 1 ]->index );
    for( auto c_iter = c_begin; c_iter != c_end; ++c_iter ) {
      const auto c = c_iter->second;
      const auto [a_begin, a_end] = edge_connection.equal_range( c );
      for( auto a_iter = a_begin; a_iter != a_end; ++a_iter ) {
        const auto a = a_iter->second;
        if( a == delaunay_edge.sites[ 0 ]->index ) {
          const auto t = glm::ivec3{
            delaunay_edge.sites[ 0 ]->index,
            delaunay_edge.sites[ 1 ]->index,
            c
          };
          const auto sorted = sort_element( t );
          const auto known = known_triangle.find( sorted );
          if( known != known_triangle.end() ) {
            ++known->second;
          }
          else {
            known_triangle[ sorted ] = 1u;
            triangle.push_back( t );
          }
        }
      }
    }
  }
  

 /* 
    const auto sorted = glm::ivec2{
      std::min( e.x, e.y ),
      std::max( e.x, e.y )
    };
    jcv_edge_iter edge_b_iter;
    jcv_edge edge_b;
    jcv_site_get_edges( &diagram, &sites[ delaunay_edge.sites[ 1 ]->index ], &edge_b_iter );
    while( jcv_edge_next( &edge_b_iter, &edge_b ) ) {
      jcv_edge_iter edge_c_iter;
      jcv_edge edge_c;
      if( edge_b.sites[ 1 ] ) {
        jcv_site_get_edges( &diagram, &sites[ edge_b.sites[ 1 ]->index ], &edge_c_iter );
        while( jcv_edge_next( &edge_c_iter, &edge_c ) ) {
          if( edge_c.sites[ 1 ] && edge_c.sites[ 1 ]->index == delaunay_edge.sites[ 0 ]->index ) {
            const auto t = glm::ivec3{
              delaunay_edge.sites[ 0 ]->index,
              delaunay_edge.sites[ 1 ]->index,
              edge_b.sites[ 1 ]->index
            };
            const auto sorted = sort_element( t );
            const auto known = known_triangle.find( sorted );
            if( known != known_triangle.end() ) {
              ++known->second;
            }
            else {
              known_triangle[ sorted ] = 1u;
              triangle.push_back( t );
            }
          }
        }
      }
    }
  }*/
  std::unordered_map< glm::ivec2, std::size_t > known_edge;
  for( const auto &t: triangle ) {
    {
      const auto sorted = sort_element( glm::ivec2{ t.x, t.y } );
      const auto known = known_edge.find( sorted );
      if( known != known_edge.end() ) {
        ++known->second;
      }
      else {
        known_edge[ sorted ] = 1u;
      }
    }
    {
      const auto sorted = sort_element( glm::ivec2{ t.y, t.z } );
      const auto known = known_edge.find( sorted );
      if( known != known_edge.end() ) {
        ++known->second;
      }
      else {
        known_edge[ sorted ] = 1u;
      }
    }
    {
      const auto sorted = sort_element( glm::ivec2{ t.z, t.x } );
      const auto known = known_edge.find( sorted );
      if( known != known_edge.end() ) {
        ++known->second;
      }
      else {
        known_edge[ sorted ] = 1u;
      }
    }
  }
  for( const auto &e: known_edge ) {
    if( e.second == 1u ) {
      vertex[ e.first.x ].border = true;
      vertex[ e.first.y ].border = true;
    }
  }
  for( unsigned int i = 0u; i != vertex.size(); ++i ) {
    vertex[ i ].full = add_vertex( graph );
    graph[ vertex[ i ].full ] = i;
  }
  for( const auto &e: edge ) {
    add_edge( vertex[ e.x ].full, vertex[ e.y ].full, graph );
  }
}

void terrain_mesh::uplift() {
  for( auto &v: vertex ) {
    v.position.z += v.uplift;
  }
}

namespace {

struct MarkRootVisitor : public boost::default_bfs_visitor {
  MarkRootVisitor(
    terrain_mesh &m_,
    terrain_river_tree_type::vertex_descriptor r_
  ) : mesh( m_ ), root( r_ ) {}
  void discover_vertex( terrain_river_tree_type::vertex_descriptor u, const terrain_river_tree_type & g ) const {
    assert( !mesh.vertex[ g[ u ] ].root );
    //std::cout << "vertex " << g[ u ] << " root " << root << std::endl;
    mesh.vertex[ g[ u ] ].root = root;
  }
  terrain_mesh &mesh;
  terrain_river_tree_type::vertex_descriptor root;
};

}

float terrain_mesh::calculate_area_sum( std::uint32_t start_index ) {
  float sum = vertex[ start_index ].area;
  const auto [edge_begin,edge_end] = out_edges( *vertex[ start_index ].river, river );
  for( auto iter = edge_begin; iter != edge_end; ++iter ) {
    const auto next_desc = target( *iter, river );
    const auto next_index = river[ next_desc ];
    //std::cout << start_index << " -> " << next_index << std::endl;
    sum += calculate_area_sum( river[ next_desc ] );
  }
  //std::cout << "vertex : " << start_index << " sum : " << sum << " self : " << vertex[ start_index ].area << std::endl;
  //std::cout << "visited : " << start_index << std::endl;
  vertex[ start_index ].area_sum = sum;
  return sum;
}


void terrain_mesh::generate_river_tree() {
  river = terrain_river_tree_type();
  lake = terrain_full_graph_type();
  //saturate = terrain_river_tree_type();
  for( unsigned int i = 0u; i != vertex.size(); ++i ) {
    vertex[ i ].river = add_vertex( river );
    vertex[ i ].root = std::nullopt;
    vertex[ i ].lake = std::nullopt;
    //vertex[ i ].saturate = std::nullopt;
    river[ *vertex[ i ].river ] = i;
  }
  lake_index.clear();
  for( unsigned int i = 0u; i != vertex.size(); ++i ) {
    auto [out_begin,out_end] = out_edges( vertex[ i ].full, graph );
    std::optional< std::uint32_t > lowest_vertex;
    float lowest_height_diff = 0.0f;
    const float source_height = vertex[ i ].position.z;
    for( auto edge_iter = out_begin; edge_iter != out_end; ++edge_iter ) {
      const auto target_desc = target( *edge_iter, graph );
      const auto target_index = graph[ target_desc ];
      const auto target_height = vertex[ target_index ].position.z;
      float height_diff = target_height - source_height;
      if( height_diff < lowest_height_diff ) {
        lowest_height_diff = height_diff;
        lowest_vertex = target_index;
      }
    }
    if( lowest_vertex ) {
      assert( vertex[ *lowest_vertex ].river );
      assert( vertex[ i ].river );
      add_edge( *vertex[ *lowest_vertex ].river, *vertex[ i ].river, river );
    }
    else {
      lake_index.insert( i );
      vertex[ i ].lake = add_vertex( lake );
      //vertex[ i ].saturate = add_vertex( saturate );
      lake[ *vertex[ i ].lake ] = i;
      //saturate[ *vertex[ i ].saturate ] = i;
    }
  }

  for( const auto &li: lake_index ) {
    assert( vertex[ li ].river );
    const auto start = *vertex[ li ].river;
    boost::breadth_first_search(
      river,
      start,
      visitor( MarkRootVisitor{ *this, start } )
    );
  }

  lowest_pair.clear();
 
  {
    auto [vertex_begin,vertex_end] = vertices( graph );
    for( auto from_iter = vertex_begin; from_iter != vertex_end; ++from_iter ) {
      const auto from_index = graph[ *from_iter ];
      assert( vertex[ from_index ].root );
      const auto from_lake_index = river[ *vertex[ from_index ].root ];
      const auto from_height = vertex[ from_index ].position.z;
      auto [edge_begin,edge_end] = out_edges( *from_iter, graph );
      for( auto edge_iter = edge_begin; edge_iter != edge_end; ++edge_iter ) {
        auto to_desc = target( *edge_iter, graph );
        const auto to_index = graph[ to_desc ];
        assert( vertex[ to_index ].root );
        const auto to_lake_index = river[ *vertex[ to_index ].root ];
        if( from_lake_index != to_lake_index ) {
          const auto to_height = vertex[ to_index ].position.z;
          const auto height = std::max( from_height, to_height );
          const auto key = std::make_pair( std::min( from_lake_index, to_lake_index ), std::max( from_lake_index, to_lake_index ) );
          auto existing = lowest_pair.find( key );
          if( existing != lowest_pair.end() ) {
            if( existing->second.first > height ) {
              existing->second.first = height;
              existing->second.second = *edge_iter;
            }
          }
          else {
            lowest_pair[ key ].first = height;
            lowest_pair[ key ].second = *edge_iter;
          }
        }
      }
    }
  }

  for( auto &p: lowest_pair ) {
    const auto from = vertex[ p.first.first ].lake;
    const auto to = vertex[ p.first.second ].lake;
    assert( from );
    assert( to );
    add_edge( *from, *to, lake );
  }

  //std::cout << "lake count 1 " << lake_index.size() << std::endl;
  {
    auto [vertex_begin,vertex_end] = vertices( lake );
    for( auto from_lake_iter = vertex_begin; from_lake_iter != vertex_end; ++from_lake_iter ) {
      const auto from_lake_desc = *from_lake_iter;
      const auto from_lake_index = lake[ from_lake_desc ];
      if( !vertex[ from_lake_index ].border ) {
        const auto from_lake_height = vertex[ from_lake_index ].position.z;
        const auto [edge_begin,edge_end] = out_edges( from_lake_desc, lake );
        float lowest_height_diff = std::numeric_limits< float >::max();
        std::optional< terrain_full_graph_type::edge_descriptor > lowest_edge = std::nullopt;
        for( auto edge_iter = edge_begin; edge_iter != edge_end; ++edge_iter ) {
          const auto to_lake_desc = target( *edge_iter, lake );
          const auto to_lake_index = lake[ to_lake_desc ];
          const auto key = std::make_pair( std::min( from_lake_index, to_lake_index ), std::max( from_lake_index, to_lake_index ) );
          const auto path = lowest_pair.find( key );
          assert( path != lowest_pair.end() );
          auto a_index = graph[ source( path->second.second, graph ) ];
          auto b_index = graph[ target( path->second.second, graph ) ];
          assert( vertex[ a_index ].root );
          assert( vertex[ b_index ].root );
          auto a_root_index = river[ *vertex[ a_index ].root ];
          auto b_root_index = river[ *vertex[ b_index ].root ];
          assert( a_root_index != b_root_index );
          if( b_root_index == from_lake_index ) {
            std::swap( a_index, b_index );
            std::swap( a_root_index, b_root_index );
          }
          const auto to_height = vertex[ b_index ].position.z;
          const auto height_diff = to_height - from_lake_height;
          //std::cout << "lowest_height_diff : " << lowest_height_diff << " height_diff : " << height_diff <<std::endl; 
          if( lowest_height_diff > height_diff ) {
            lowest_height_diff = height_diff;
            lowest_edge = path->second.second;
          }
        }
        if( lowest_edge ) {
          auto a_index = graph[ source( *lowest_edge, graph ) ];
          auto b_index = graph[ target( *lowest_edge, graph ) ];
          assert( vertex[ a_index ].root );
          assert( vertex[ b_index ].root );
          auto a_root_index = river[ *vertex[ a_index ].root ];
          auto b_root_index = river[ *vertex[ b_index ].root ];
          assert( a_root_index != b_root_index );
          if( b_root_index == from_lake_index ) {
            std::swap( a_index, b_index );
            std::swap( a_root_index, b_root_index );
          }
          const auto to_lake_height = vertex[ b_root_index ].position.z;
          //assert( vertex[ from_lake_index ].saturate );
          //assert( vertex[ b_root_index ].saturate );
          //add_edge( *vertex[ from_lake_index ].saturate, *vertex[ b_root_index ].saturate, saturate );
          assert( vertex[ from_lake_index ].river );
          assert( vertex[ b_index ].river );
          if( to_lake_height < from_lake_height ) {
          //if( !boost::edge( *vertex[ a_index ].river, *vertex[ b_root_index ].river, river ).second ) { ///////
            //std::cout << "connect lake " << from_lake_index << " to " << b_root_index << " : "  << b_index << std::endl;
            add_edge( *vertex[ b_index ].river, *vertex[ from_lake_index ].river, river );
            lake_index.erase( from_lake_index );
          }
        }
      }
    }
  }
  //std::cout << "lake count 2 " << lake_index.size() << std::endl;

  {
    float sum = 0.0f;
    for( const auto &li: lake_index ) {
      //std::cout << "init " << li << std::endl;
      sum += calculate_area_sum( li );
    }
    //std::cout << "total " << sum << std::endl;
    assert( sum > 0.99f * props.distance_scale.x * props.distance_scale.y );
    assert( sum < 1.01f * props.distance_scale.x * props.distance_scale.y );
  }

  /*std::vector< jcv_point > lake_points;
  for( auto &l: lake_index ) {
    jcv_point p;
    p.x = vertex[ l ].position.x;
    p.y = vertex[ l ].position.y;
    lake_points.push_back( p );
  }
  jcv_rect bounding_box = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
  jcv_diagram diagram;
  memset(&diagram, 0, sizeof(jcv_diagram));
  jcv_diagram_generate( lake_points.size(), lake_points.data(), &bounding_box, 0, &diagram );

  jcv_delaunay_iter delaunay;
  jcv_delaunay_edge delaunay_edge;

  std::unordered_multimap< std::size_t, std::size_t > edge_connection;

  jcv_delaunay_begin( &diagram, &delaunay );
  while( jcv_delaunay_next( &delaunay, &delaunay_edge ) ) {
    add_edge(
      vertex[ lake_index[ delaunay_edge.sites[ 0 ]->index ] ].lake,
      vertex[ lake_index[ delaunay_edge.sites[ 1 ]->index ] ].lake,
      lake
    );
  }
  const auto [sat_begin,sat_end] = vertices( saturate );
  std::vector< terrain_river_tree_type::vertex_descriptor > current;
  std::vector< terrain_river_tree_type::vertex_descriptor > next;
  for( auto iter = sat_begin; iter != sat_end; ++iter ) {
    if( vertex[ lake[*iter] ].border ) {
      next.push_back( *iter );
    }
  }
  while( !next.empty() ) {
    std::swap( current, next );
    next.clear();
    for( auto &cs_desc: current ) {
      const auto cl_desc = vertex[ saturate[ cs_desc ] ].lake;
      const auto [next_lake_begin,next_lake_end] = out_edges( cl_desc, lake );
      for( auto nl_iter = next_lake_begin; nl_iter != next_lake_end; ++nl_iter ) {
        const auto nl_desc = target( *nl_iter, lake );
        const bool is_border = vertex[ lake[ nl_desc ] ].border;
        const float center_height = vertex[ lake[ nl_desc ] ].position.z;
        const auto ns_desc = vertex[ lake[ nl_desc ] ].saturate;
        const auto [existing_begin,existing_end] = out_edges( ns_desc, saturate );
        if( !is_border && existing_begin == existing_end ) {
          const auto [candidate_begin,candidate_end] = out_edges( nl_desc, lake );
          float lowest_height_diff = std::numeric_limits< float >::max();
          auto lowest_neighbor = nl_desc;
          for( auto nnl_iter = candidate_begin; nnl_iter != candidate_end; ++nnl_iter ) {
            const auto nnl_desc = target( *nnl_iter, lake );
            const float neighbor_height = vertex[ lake[ nnl_desc ] ].position.z;
            const float height_diff = center_height - neighbor_height;
            if( height_diff < lowest_height_diff ) {
              lowest_height_diff = height_diff;
              lowest_neighbor = target( *nnl_iter, lake );
            }
          }
          if( lowest_neighbor == cl_desc ) {
            add_edge( ns_desc, cs_desc, saturate );
            next.push_back( ns_desc );
          }
        }
      }
    }
  }

  const
*/

  /////////
/*
  const auto sites = jcv_diagram_get_sites( &diagram );
  vertex.resize( diagram.numsites );
  for( std::size_t i = 0u; i < diagram.numsites; ++i ) {
    vertex[ sites[ i ].index ].set_position( glm::vec3{ sites[ i ].p.x, sites[ i ].p.y, 0.0f } );
    
    jcv_edge_iter edge_iter;
    jcv_edge graph_edge;
    jcv_site_get_edges(&diagram, &sites[i], &edge_iter);
    std::vector< glm::vec2 > voronoi_vertex;
    while (jcv_edge_next(&edge_iter, &graph_edge)) {
      voronoi_vertex.push_back( glm::vec2( graph_edge.pos[ 0 ].x, graph_edge.pos[ 0 ].y ) );
    }
    vertex[ sites[ i ].index ].set_area( gct::area( voronoi_vertex ) );
  }




  for( const auto &e: edge ) {
    if( vertex[ e.x ].position.z < vertex[ e.y ].position.z ) {
    
    }
    add_edge( vertex[ e.x ].full, vertex[ e.y ].full, graph );
  }
  */
}

namespace {


struct ErosionVisitor : public boost::default_bfs_visitor {
  ErosionVisitor(
    terrain_mesh &m_,
    terrain_river_tree_type::vertex_descriptor r_
  ) : mesh( m_ ), root( r_ ) {}
  void tree_edge( const terrain_river_tree_type::edge_descriptor &e, const terrain_river_tree_type &g ) {
    //std::cout << mesh.river[ source( e, g ) ] << " -> " << mesh.river[ target( e, g ) ] << " " << mesh.vertex[ mesh.river[ target( e, g ) ] ].area_sum << std::endl;
    const float k = mesh.get_props().erosion_rate;
    const float ami = mesh.vertex[ mesh.river[ target( e, g ) ] ].area_sum;
    const float ui = mesh.vertex[ mesh.river[ target( e, g ) ] ].uplift;
    const auto pi = mesh.vertex[ mesh.river[ target( e, g ) ] ].position;
    const auto pj = mesh.vertex[ mesh.river[ source( e, g ) ] ].position;
    const float d = glm::distance( glm::vec2( pi ), glm::vec2( pj ) );
    const float delta_t = mesh.get_props().delta_t;
    //if( pi.z > pj.z ) {
      const float new_h = ( pi.z + delta_t * ( ui + k * ami / d ) ) / ( 1.0f + ( k * ami / d ) * delta_t );
      //const float h_diff = new_h - pj.z;
      //const float r = ( h_diff / d );
      //if( r > 1.7320508075688767f ) {
      //}
      //else if( r > 0.5773502691896256f ) {
      //  const auto ratio = ( r - 0.5773502691896256f ) / 1.154700538379251f;
      //  mesh.vertex[ mesh.river[ target( e, g ) ] ].position.z =
      //    mesh.vertex[ mesh.river[ target( e, g ) ] ].position.z * ( 1.0f - ratio ) +
      //    new_h * ratio;
      //}
      //else {
        mesh.vertex[ mesh.river[ target( e, g ) ] ].position.z = new_h;
      //}
    //}
    //else {
    //  const float new_h = ( pi.z + delta_t * ( ui + k * ami / d ) ) / ( 1.0f + ( k * ami / d ) * delta_t );
    //  mesh.vertex[ mesh.river[ target( e, g ) ] ].position.z = new_h;
    //}

  }
  terrain_mesh &mesh;
  terrain_river_tree_type::vertex_descriptor root;
};

struct ThermalErosionVisitor : public boost::default_bfs_visitor {
  ThermalErosionVisitor(
    terrain_mesh &m_,
    terrain_river_tree_type::vertex_descriptor r_
  ) : mesh( m_ ), root( r_ ) {}
  void tree_edge( const terrain_river_tree_type::edge_descriptor &e, const terrain_river_tree_type &g ) {
    const auto pi = mesh.vertex[ mesh.river[ target( e, g ) ] ].position;
    const auto pj = mesh.vertex[ mesh.river[ source( e, g ) ] ].position;
    const float d = glm::distance( glm::vec2( pi ), glm::vec2( pj ) );
    const float h_diff = pi.z - pj.z;
    const float r = ( h_diff / ( d * 1000.f ) );
    if( r > 0.5773502691896256f ) {
      const float ratio = ( r - 0.5773502691896256f ) / ( 1.7320508075688767f - 0.5773502691896256f );
       mesh.vertex[ mesh.river[ target( e, g ) ] ].position.z =
         pj.z + 0.5773502691896256f * d;
         //( pj.z + 0.5773502691896256f * d ) * ratio +
         //pi.z * ( 1.0f - ratio );
    }
    else if( r < -0.5773502691896256f ) {
      const float ratio = ( -r - 0.5773502691896256f ) / ( 1.7320508075688767f - 0.5773502691896256f );
       mesh.vertex[ mesh.river[ target( e, g ) ] ].position.z =
         pj.z - 0.5773502691896256f * d;
         //( pj.z - 0.5773502691896256f * d ) * ratio +
         //pi.z * ( 1.0f - ratio );
    }
  }
  terrain_mesh &mesh;
  terrain_river_tree_type::vertex_descriptor root;
};


}



void terrain_mesh::erosion() {
  for( auto &li: lake_index ) {
    vertex[ li ].position.z += props.delta_t * vertex[ li ].uplift;
    boost::breadth_first_search(
      river,
      *vertex[ li ].river,
      visitor( ErosionVisitor{ *this, *vertex[ li ].river } )
    );
    boost::breadth_first_search(
      river,
      *vertex[ li ].river,
      visitor( ThermalErosionVisitor{ *this, *vertex[ li ].river } )
    );
  }
}

void terrain_mesh::rasterize( host_image &dest ) {
  for( const auto &t: triangle ) {
    const glm::vec3 p0 = vertex[ t.x ].position;
    const glm::vec3 p1 = vertex[ t.y ].position;
    const glm::vec3 p2 = vertex[ t.z ].position;

    const glm::vec2 s0 = glm::vec2(
      p0.x / props.distance_scale.x * dest.width,
      p0.y / props.distance_scale.y * dest.height
    ) - glm::vec2( 0.5f, 0.5f );
    const glm::vec2 s1 = glm::vec2(
      p1.x / props.distance_scale.x * dest.width,
      p1.y / props.distance_scale.y * dest.height
    ) - glm::vec2( 0.5f, 0.5f );
    const glm::vec2 s2 = glm::vec2(
      p2.x / props.distance_scale.x * dest.width,
      p2.y / props.distance_scale.y * dest.height
    ) - glm::vec2( 0.5f, 0.5f );
    glm::vec2 s_min = glm::min( glm::min( s0, s1 ), s2 );
    glm::vec2 s_max = glm::max( glm::max( s0, s1 ), s2 );
    s_min = glm::max( s_min, glm::vec2( 0, 0 ) );
    s_max = glm::min( s_max, glm::vec2( dest.width, dest.height ) );
  
    const glm::ivec2 left_top_pixel = glm::ivec2( ceil( s_min ) );
    const glm::ivec2 bounding_box_pixel_count =
      glm::ivec2( ceil( s_max ) ) - left_top_pixel;
 
    const glm::vec2 v_ab = s1 - s0;
    const glm::vec2 v_ac = s2 - s0;
 
    const float factor = 1.0f / rasterization_cross2( v_ab, v_ac );
    //std::cout << "factor " << factor << std::endl; 
    const float ds_dx =  v_ac.y * factor;
    const float ds_dy = -v_ac.x * factor;
    const float dt_dx = -v_ab.y * factor;
    const float dt_dy =  v_ab.x * factor;
    //std::cout << "ds_dx " << ds_dx << " " << ds_dy << " " << dt_dx << " " << dt_dy << std::endl; 

    // bounding box内での左上のピクセルの位置
    const glm::vec2 bounding_box_left_top_pixel_pos = glm::vec2(
      left_top_pixel.x - s0.x,
 	    left_top_pixel.y - s0.y
    );
 
    const glm::vec2 left_top_pixel_pos_barycentric = glm::vec2(
      ( bounding_box_left_top_pixel_pos.x * v_ac.y - bounding_box_left_top_pixel_pos.y * v_ac.x ) * factor,
      ( v_ab.x * bounding_box_left_top_pixel_pos.y - v_ab.y * bounding_box_left_top_pixel_pos.x ) * factor
    );

    for( std::uint32_t y = 0u; y != bounding_box_pixel_count.y; ++y ) {
      for( std::uint32_t x = 0u; x != bounding_box_pixel_count.x; ++x ) {
        const glm::ivec2 pixel_in_bounding_box = glm::ivec2( x, y );
        const float s = left_top_pixel_pos_barycentric.x +
                        pixel_in_bounding_box.x * ds_dx +
                        pixel_in_bounding_box.y * ds_dy;
 	 	    const float t = left_top_pixel_pos_barycentric.y +
                        pixel_in_bounding_box.x * dt_dx +
                        pixel_in_bounding_box.y * dt_dy;
   
        const float v = 1.0f - ( s + t );
        if( s >= 0.0f && t >= 0.0f && v >= 0.0f ) {
          float v_ds = p1.z - p0.z;
          float v_dt = p2.z - p0.z;
          dest.image[ ( left_top_pixel.x + x ) + ( left_top_pixel.y + y ) * dest.width ] = p0.z + s * v_ds + t * v_dt;
          //dest.image[ 0 + ( left_top_pixel.x + x ) * dest.channels + ( left_top_pixel.y + y ) * dest.width * dest.channels ] = s;
          //dest.image[ 1 + ( left_top_pixel.x + x ) * dest.channels + ( left_top_pixel.y + y ) * dest.width * dest.channels ] = t;
        }
      }
    }
  }
}

void to_json( nlohmann::json &dest, const terrain_mesh &src ) {
  dest = nlohmann::json::object();
  dest[ "vertex" ] = nlohmann::json::array();
  for( const auto &v: src.vertex ) {
    dest[ "vertex" ].push_back( v );
  }
  dest[ "edge" ] = nlohmann::json::array();
  for( const auto &v: src.edge ) {
    auto temp = nlohmann::json::array();
    temp.push_back( v.x );
    temp.push_back( v.y );
    dest[ "edge" ].push_back( temp );
  }
  dest[ "triangle" ] = nlohmann::json::array();
  for( const auto &v: src.triangle ) {
    auto temp = nlohmann::json::array();
    temp.push_back( v.x );
    temp.push_back( v.y );
    temp.push_back( v.z );
    dest[ "triangle" ].push_back( temp );
  }
}
/*
void from_json( const nlohmann::json &src, terrain_mesh &dest ) {
  if( !src.is_object() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
  dest = terrain_mesh{};
  if( src.find( "vertex" ) != src.end() ) {
    if( !src[ "vertex" ].is_array() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
    for( const auto &v: src[ "vertex" ] ) {
      dest.vertex.push_back( v );
    }
  }
  if( src.find( "edge" ) != src.end() ) {
    if( !src[ "edge" ].is_array() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
    for( const auto &v: src[ "edge" ] ) {
      if( !v.is_array() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( v.size() != 2u ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( std::size_t( v[ 0 ] ) >= dest.vertex.size() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( std::size_t( v[ 1 ] ) >= dest.vertex.size() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      dest.edge.push_back( glm::ivec2{ std::size_t( v[ 0 ] ), std::size_t( v[ 1 ] ) } );
    }
  }
  if( src.find( "triangle" ) != src.end() ) {
    if( !src[ "triangle" ].is_array() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
    for( const auto &v: src[ "triangle" ] ) {
      if( !v.is_array() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( v.size() != 3u ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( std::size_t( v[ 0 ] ) >= dest.vertex.size() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( std::size_t( v[ 1 ] ) >= dest.vertex.size() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      if( std::size_t( v[ 2 ] ) >= dest.vertex.size() ) throw incompatible_json( "The JSON is incompatible to terrain_mesh", __FILE__, __LINE__ );
      dest.triangle.push_back( glm::ivec3{ std::size_t( v[ 0 ] ), std::size_t( v[ 1 ] ), std::size_t( v[ 2 ] ) } );
    }
  }
}
*/

}


