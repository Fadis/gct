#ifndef GCT_TERRAIN_MESH_H
#define GCT_TERRAIN_MESH_H
/*
 * Copyright (C) 2026 Naomasa Matsubayashi
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */
#include <optional>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <gct/setter.hpp>
#include <nlohmann/json_fwd.hpp>
#include <boost/functional/hash.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <gct/host_image.hpp>
#include <gct/property.hpp>

namespace std {
  template<>
  struct hash< std::pair< std::uint32_t, std::uint32_t > > {
    std::size_t operator()( const std::pair< std::uint32_t, std::uint32_t > &v ) const {
      return std::hash< std::uint32_t >{}( v.first ) ^ std::hash< std::uint32_t >{}( v.second );
    }
  };
}

namespace gct {

  using terrain_full_graph_type = boost::adjacency_list<
    boost::hash_setS,
    boost::vecS,
    boost::undirectedS,
    std::size_t
  >;
  using terrain_river_tree_type = boost::adjacency_list<
    boost::hash_setS,
    boost::vecS,
    boost::bidirectionalS,
    std::size_t
  >;

struct terrain_mesh_create_info {
  LIBGCT_SETTER( point_count )
  LIBGCT_SETTER( distance_scale )
  LIBGCT_SETTER( max_uplift )
  LIBGCT_SETTER( erosion_rate )
  LIBGCT_SETTER( delta_t )
  LIBGCT_SETTER( uniform_grid )
  LIBGCT_SETTER( stochastic )
  LIBGCT_SETTER( connect_lake )
  LIBGCT_SETTER( uplift )
  LIBGCT_SETTER( previous_height )
  LIBGCT_SETTER( initial_height )
  std::size_t point_count = 10000;
  glm::vec2 distance_scale = glm::vec2{ 50.0f*1000.f, 50.0f*1000.f };
  float max_uplift = 5.0e-3f; //5.0e-4f; // u
  float erosion_rate = 5.61e-10f;// * 2.5e+5f; // k
  float delta_t = 2.5e+5;//  2.5e+5/(1000.f);
  bool uniform_grid = false;
  bool stochastic = false;
  bool connect_lake = true;
  host_image uplift;
  host_image previous_height;
  host_image initial_height;
};

struct terrain_vertex {
  LIBGCT_SETTER( position )
  LIBGCT_SETTER( uplift )
  LIBGCT_SETTER( area )
  LIBGCT_SETTER( area_sum )
  LIBGCT_SETTER( border )
  LIBGCT_SETTER( previous_height )
  LIBGCT_SETTER( t )
  LIBGCT_SETTER( d )
  LIBGCT_SETTER( s )
  LIBGCT_SETTER( x )
  LIBGCT_SETTER( dx0t )

  LIBGCT_SETTER( full )
  LIBGCT_SETTER( river )
  LIBGCT_SETTER( root )
  LIBGCT_SETTER( lake )
  //LIBGCT_SETTER( saturate )
  glm::vec3 position = glm::vec3{ 0.0f, 0.0f, 0.0f };
  float uplift = 0.0f;
  float area = 0.0f;
  float area_sum = 0.0f;
  bool border = false;
  float previous_height = 0.0f;
  // pbae
  float t = 0.0f;
  float d = 0.0f;
  float s = 0.0f;
  std::uint32_t x = 0u;
  bool dx0t = false;
  float optimized_height = 0.0f;
  std::unordered_set< std::uint32_t > ncn;
  terrain_full_graph_type::vertex_descriptor full;
  std::optional< terrain_river_tree_type::vertex_descriptor > river;
  std::optional< terrain_river_tree_type::vertex_descriptor > root;
  std::optional< terrain_full_graph_type::vertex_descriptor > lake;
  //std::optional< terrain_river_tree_type::vertex_descriptor > saturate;
};

void to_json( nlohmann::json&, const terrain_vertex& );
void from_json( const nlohmann::json&, terrain_vertex& );

struct terrain_mesh :
  public property< terrain_mesh_create_info > {
  terrain_mesh( const terrain_mesh_create_info &create_info );
  void uplift();
  void erosion();
  void erosion( std::uint32_t t );
  void generate_river_tree();
  float calculate_area_sum( std::uint32_t start_index );
  void dump() const;
  void dump_triangle( const std::string &filename );
  void dump_vertex( const std::string &filename );
  void dump_edge( const std::string &filename );
  void dump_river( const std::string &filename );
  void dump_lake( const std::string &filename );
  void rasterize( host_image &dest );
  float get_loss();
  float get_loss_grad( std::uint32_t g );
  void optimize( std::uint32_t c );
  void generate_ncn();
  std::vector< terrain_vertex > vertex;
  std::vector< glm::ivec2 > edge;
  std::vector< glm::ivec3 > triangle;
  terrain_full_graph_type graph;
  terrain_river_tree_type river;
  terrain_full_graph_type lake;
  std::unordered_map< std::pair< std::uint32_t, std::uint32_t >, std::pair< float, terrain_full_graph_type::edge_descriptor > > lowest_pair;
  //terrain_river_tree_type saturate;
  std::unordered_set< std::uint32_t > lake_index;
};

void to_json( nlohmann::json&, const terrain_mesh& );
//void from_json( const nlohmann::json&, terrain_mesh& );

}

#endif

