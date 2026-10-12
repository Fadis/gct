#include <iostream>
#include <unordered_map>
#include <nlohmann/json.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>
#include <gct/gaussian_blue_noise.hpp>
#include <gct/terrain_mesh.hpp>
#include <gct/area.hpp>
#include <gct/extension.hpp>
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

terrain_mesh::terrain_mesh( std::size_t vertex_count, float area ) {
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


