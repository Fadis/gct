#ifndef GCT_SHADER_GRAPH_VERTEX_HPP
#define GCT_SHADER_GRAPH_VERTEX_HPP

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/property_map/property_map.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/image_io_create_info.hpp>

namespace gct {

class compute;
class image_view_t;

namespace scene_graph {
  class scene_graph_resource;
}

using shader_graph_vertex_list = std::vector< image_io_create_info >;

struct shader_graph_subedge {
  LIBGCT_SETTER( from )
  LIBGCT_SETTER( to )
  std::string from;
  std::string to;
};

using shader_graph_subedge_list = std::vector< shader_graph_subedge >;

struct shader_graph_vertex_type {
  LIBGCT_SETTER( create_info )
  LIBGCT_SETTER( color )
  std::shared_ptr< image_io_create_info > create_info;
  boost::default_color_type color = boost::white_color; 
};

using shader_graph_graph_type =  boost::adjacency_list<
  boost::listS,
  boost::listS,
  boost::directedS,
  shader_graph_vertex_type,
  shader_graph_subedge_list
>;


class shader_graph_vertex {
public:
  using graph_type = shader_graph_graph_type;
  struct subresult_type {
    LIBGCT_SETTER( vertex_id )
    LIBGCT_SETTER( create_info )
    LIBGCT_SETTER( name )
    graph_type::vertex_descriptor vertex_id;
    std::shared_ptr< image_io_create_info > create_info;
    std::string name;
  };
  class result_type {
  public:
    result_type(
      const std::shared_ptr< graph_type > &g,
      graph_type::vertex_descriptor id,
      const std::shared_ptr< image_io_create_info > &ci
    ) : graph( g ), vertex_id( id ), create_info( ci ) {}
    subresult_type operator[]( const std::string name ) const;
  private:
    std::shared_ptr< graph_type > graph;
    graph_type::vertex_descriptor vertex_id;
    std::shared_ptr< image_io_create_info > create_info;
  };
  shader_graph_vertex(
    const std::shared_ptr< graph_type > &g,
    graph_type::vertex_descriptor id,
    const std::shared_ptr< image_io_create_info > &ci
  ) : graph( g ), vertex_id( id ), create_info( ci ) {}
  result_type operator()(
    const std::unordered_map< std::string, subresult_type > &input
  );
private:
  std::shared_ptr< graph_type > graph;
  graph_type::vertex_descriptor vertex_id;
  std::shared_ptr< image_io_create_info > create_info;
  bool called = false;
};

class shader_graph_optimizer {
public:
  using graph_type = shader_graph_graph_type;
private:
  struct image_binding {
    LIBGCT_SETTER( allocate_info )
    LIBGCT_SETTER( view )
    LIBGCT_SETTER( used_by )
    std::shared_ptr< image_allocate_info > allocate_info;
    image_pool::image_descriptor view;
    std::vector< std::pair< graph_type::vertex_descriptor, std::string > > used_by;
  };
public:
  shader_graph_optimizer() : graph( new graph_type() ) {
  }
  auto create( const image_io_create_info &v ) {
    auto desc = add_vertex( *graph );
    auto shared_v = std::make_shared< image_io_create_info >( v );
    (*graph)[ desc ].set_create_info( shared_v );
    return shader_graph_vertex( graph, desc, shared_v );
  }
private:
  std::vector< std::pair< graph_type::vertex_descriptor, std::string > > get_consumer_of(
    const graph_type::vertex_descriptor &, const std::string&
  ) const;
  bool shareable(
    const graph_type::vertex_descriptor &,
    const std::string&,
    const graph_type::vertex_descriptor &
  ) const;
  bool shareable(
    const std::vector< std::pair< graph_type::vertex_descriptor, std::string > > &first_generator,
    const graph_type::vertex_descriptor &second_generator
  ) const;
  void assign_image();
  std::shared_ptr< graph_type > graph;
  std::vector< image_binding > binding;
};

}

#endif

