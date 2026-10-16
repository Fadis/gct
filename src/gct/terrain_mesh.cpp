#include <iostream>
#include <unordered_map>
#include <boost/graph/breadth_first_search.hpp>
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <gct/gaussian_blue_noise.hpp>
#include <gct/terrain_mesh.hpp>
#include <gct/area.hpp>
#include <gct/extension.hpp>
#include <gct/linear_interpolation.hpp>
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

terrain_mesh::terrain_mesh( std::size_t vertex_count, float area, const host_image &uplift ) {
  auto result = gct::gaussian_blue_noise( vertex_count, 1000 );
  std::vector< jcv_point > points;
  for( auto &v: result ) {
    jcv_point p;
    p.x = v.x;
    p.y = v.y;
    points.push_back( p );
  }
  jcv_rect bounding_box = { { 0.0f, 0.0f }, { 1.0f, 1.0f } };
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
      linear_interpolation( uplift, 0u, glm::vec2( vertex[ sites[ i ].index ].position ) )
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
    std::cout << "vertex " << g[ u ] << " root " << root << std::endl;
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
    std::cout << start_index << " -> " << next_index << std::endl;
    sum += calculate_area_sum( river[ next_desc ] );
  }
  //std::cout << "vertex : " << start_index << " sum : " << sum << " self : " << vertex[ start_index ].area << std::endl;
  std::cout << "visited : " << start_index << std::endl;
  vertex[ start_index ].area_sum = sum;
  return sum;
}


void terrain_mesh::generate_river_tree() {
  river = terrain_river_tree_type();
  lake = terrain_full_graph_type();
  saturate = terrain_river_tree_type();
  for( unsigned int i = 0u; i != vertex.size(); ++i ) {
    vertex[ i ].river = add_vertex( river );
    vertex[ i ].root = std::nullopt;
    vertex[ i ].lake = std::nullopt;
    vertex[ i ].saturate = std::nullopt;
    river[ *vertex[ i ].river ] = i;
  }
  std::unordered_set< std::uint32_t > lake_index;
  std::unordered_set< std::uint32_t > border_lake_index;
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
      if( vertex[ i ].border ) {
        border_lake_index.insert( i );
      }
      vertex[ i ].lake = add_vertex( lake );
      vertex[ i ].saturate = add_vertex( saturate );
      lake[ *vertex[ i ].lake ] = i;
      saturate[ *vertex[ i ].saturate ] = i;
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

  std::cout << "lake count 1 " << lake_index.size() << std::endl;
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
          std::cout << "lowest_height_diff : " << lowest_height_diff << " height_diff : " << height_diff <<std::endl; 
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
          assert( vertex[ from_lake_index ].saturate );
          assert( vertex[ b_root_index ].saturate );
          add_edge( *vertex[ from_lake_index ].saturate, *vertex[ b_root_index ].saturate, saturate );
          assert( vertex[ from_lake_index ].river );
          assert( vertex[ b_index ].river );
          if( to_lake_height < from_lake_height ) {
          //if( !boost::edge( *vertex[ a_index ].river, *vertex[ b_root_index ].river, river ).second ) { ///////
            std::cout << "connect lake " << from_lake_index << " to " << b_root_index << " : "  << b_index << std::endl;
            add_edge( *vertex[ b_index ].river, *vertex[ from_lake_index ].river, river );
            lake_index.erase( from_lake_index );
          }
        }
      }
    }
  }
  std::cout << "lake count 2 " << lake_index.size() << std::endl;

  {
    float sum = 0.0f;
    for( const auto &li: lake_index ) {
      std::cout << "init " << li << std::endl;
      sum += calculate_area_sum( li );
    }
    std::cout << "total " << sum << std::endl;
    assert( sum > 0.99f );
    assert( sum < 1.01f );
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

}


