#ifndef GCT_IMAGE_FILTER2_CREATE_INFO_HPP
#define GCT_IMAGE_FILTER2_CREATE_INFO_HPP

#include <memory>
#include <vector>
#include <filesystem>
#include <variant>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/named_resource.hpp>
#include <gct/specialization_map.hpp>
#include <gct/image_create_info.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <gct/allocator_set.hpp>
#include <gct/image_allocate_info.hpp>
#include <gct/image_pool.hpp>

namespace gct {

class image_view_t;
class descriptor_set_layout_t;
class descriptor_set_t;
class pipeline_layout_t;

namespace scene_graph {
  class scene_graph_resource;
}



/*
using image_filter2_image_definition =
  std::variant<
    image_pool::image_descriptor,
    image_allocate_info
  >;

struct image_filter2_edge {
  std::string from;
  std::string to;
};

using image_filter2_executables = std::vector< std::shared_ptr< compute > >;

struct image_filter2_vertex {
  std::size_t executable_index;
  std::unordered_set< std::string > input;
  std::unordered_set< std::string > output;
  std::unordered_set< std::string > inout;
};

using image_filter2_graph = boost::adjacency_list<
  boost::vecS,
  boost::vecS,
  boost::directedS,
  image_filter2_vertex,
  image_filter2_edge
>;

struct image_filter2_context {
  image_filter2_image_definition image;
  image_filter2_executables exec;
  image_filter2_graph graph;
  using vertex_descriptor = image_filter2_graph::vertex_descriptor;
  ? add_compute(
    const std::shared_ptr< compute > &c
  );
  ? add_image_definition(
    const image_filter2_image_definition &d
  );
  ? add_execute(
    comp,
    
    {
  );


    const std::size_t id = exec.size();
    exec.push_back( c );
    auto v = add_vertex( graph );
    graph[ v ].executable_index = id;
    graph[ v ].input = input;
    graph[ v ].output = output;
    graph[ v ].inout = inout;
    return v;
  }
  void set_image_info(
    
    image_filter2_image_definition
  ) {
  }
   call(
    vertex_descriptor what,
    const std::unordered_map< std::string, std::pair< vertex_descriptor, std::string > > &input
  ) {
  }
  auto add_input( const std::shared_ptr<  > &c ) {
    const std::size_t id = execs.size();
    execs.push_back( c );
    auto v = add_vertex( graph );
    graph[ v ].executable_index = id;
    return v;
  }


};
*/
struct image_filter2_create_info {
  LIBGCT_SETTER( allocator_set )
  LIBGCT_ALLOCATOR_SET_LEGACY_SETTER( allocator_set )
  LIBGCT_SETTER( shader )
  LIBGCT_SETTER( scene_graph )
  LIBGCT_SETTER( descriptor_set_layout )
  LIBGCT_SETTER( external_descriptor_set )
  LIBGCT_SETTER( external_pipeline_layout )
  LIBGCT_SETTER( resources )
  LIBGCT_NAMED_RESOURCE_SETTER( resources )
  LIBGCT_SETTER( ignore_unused_descriptor )
  LIBGCT_SETTER( specs )
  LIBGCT_SETTER( dim )
  LIBGCT_SETTER( reusable )
  decltype(auto) add_external_descriptor_set(
    unsigned int id,
    const std::shared_ptr< descriptor_set_t > &v
  ) {
    external_descriptor_set.insert( std::make_pair( id, v ) );
    return *this;
  }
  decltype(auto) add_descriptor_set_layout(
    unsigned int id,
    const std::shared_ptr< descriptor_set_layout_t > &v
  ) {
    descriptor_set_layout.insert( std::make_pair( id, v ) );
    return *this;
  }
  image_filter2_create_info &set_scene_graph(
    const std::shared_ptr< scene_graph::scene_graph_resource >&
  );
  decltype(auto) add_spec(
    std::uint32_t id,
    const specialization_value_type &v
  ) {
    specs.insert( std::make_pair( id, v ) );
    return *this;
  }
  decltype(auto) set_dim(
    std::int32_t x,
    std::int32_t y,
    std::int32_t z
  ) {
    dim = glm::ivec3( x, y, z );
    return *this;
  }

  allocator_set_t allocator_set;
  std::filesystem::path shader;
  std::shared_ptr< scene_graph::scene_graph_resource > scene_graph;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_layout_t > > descriptor_set_layout;
  std::unordered_map< unsigned int, std::shared_ptr< descriptor_set_t > > external_descriptor_set;
  std::shared_ptr< pipeline_layout_t > external_pipeline_layout;
  std::vector< named_resource > resources;
  bool ignore_unused_descriptor = false;
  specialization_map specs;
  glm::ivec3 dim = glm::ivec3( 0, 0, 0 );
  bool reusable = false;
};

void to_json( nlohmann::json&, const image_filter2_create_info& );

}

#endif


