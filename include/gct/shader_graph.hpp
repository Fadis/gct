#ifndef GCT_SHADER_GRAPH_HPP
#define GCT_SHADER_GRAPH_HPP

#include <iostream>
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
class image_io;
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

struct shader_graph_read_to_write_barrier {
  LIBGCT_SETTER( view )
  shader_graph_read_to_write_barrier &add_view( const image_pool::image_descriptor &v ) {
    view.push_back( v );
    return *this;
  }
  std::vector< image_pool::image_descriptor > view;
};

void to_json( nlohmann::json &dest, const shader_graph_read_to_write_barrier& );

struct shader_graph_write_to_read_barrier {
  LIBGCT_SETTER( view )
  shader_graph_write_to_read_barrier &add_view( const image_pool::image_descriptor &v ) {
    view.push_back( v );
    return *this;
  }
  std::vector< image_pool::image_descriptor > view;
};

void to_json( nlohmann::json &dest, const shader_graph_write_to_read_barrier& );

using shader_graph_command = std::variant<
  std::shared_ptr< image_io >,
  shader_graph_read_to_write_barrier,
  shader_graph_write_to_read_barrier
>;

void to_json( nlohmann::json &dest, const shader_graph_command& );

using shader_graph_command_list = std::vector<
  shader_graph_command
>;

void to_json( nlohmann::json &dest, const shader_graph_command& );

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

enum class image_mode {
  writable_without_sync = 0,
  readable_after_sync = 1,
  readable_without_sync_writable_after_sync = 2,
  reaadble_after_sync_writable_after_sync = 3
};

void to_json( nlohmann::json &dest, const image_mode& );

class compiled_shader_graph {
public:
  compiled_shader_graph(
    const std::shared_ptr< scene_graph::scene_graph_resource > &r,
    shader_graph_command_list &&l
  ) : resource( r ), list( std::move( l ) ) {}
  void operator()(
    command_buffer_recorder_t &rec
  ) const;
  const shader_graph_command_list &get_list() const {
    return list;
  }
private:
  std::shared_ptr< scene_graph::scene_graph_resource > resource;
  shader_graph_command_list list;
};

void to_json( nlohmann::json &dest, const compiled_shader_graph& );

class shader_graph_builder {
public:
  using graph_type = shader_graph_graph_type;
private:
  struct image_binding {
    image_binding() : used_by( new std::vector< std::pair< graph_type::vertex_descriptor, std::string > >() ) {}
    LIBGCT_SETTER( allocate_info )
    LIBGCT_SETTER( view )
    LIBGCT_SETTER( used_by )
    image_binding &add_used_by(
      const graph_type::vertex_descriptor &v,
      const std::string &n
    ) {
      used_by->emplace_back( v, n );
      return *this;
    }
    std::shared_ptr< image_allocate_info > allocate_info;
    image_pool::image_descriptor view;
    std::shared_ptr< std::vector< std::pair< graph_type::vertex_descriptor, std::string > > > used_by;
  };
  struct image_state {
    image_state(
      const image_binding &b
    ) : generators( b.used_by ) {}
    LIBGCT_SETTER( mode )
    LIBGCT_SETTER( expected_consumer )
    LIBGCT_SETTER( generators )
    LIBGCT_SETTER( next_generator )
    image_mode mode = image_mode::writable_without_sync;
    std::vector< graph_type::vertex_descriptor > expected_consumer;
    std::shared_ptr< std::vector< std::pair< graph_type::vertex_descriptor, std::string > > > generators;
    int next_generator = 0;
  };
  using image_state_map = std::unordered_map<
    image_pool::image_descriptor,
    image_state
  >;
public:
  shader_graph_builder(
    const std::shared_ptr< scene_graph::scene_graph_resource > &r
  ) : resource( r ), graph( new graph_type() ) {}
  auto get_image_io_create_info(
    const std::shared_ptr< compute > &e,
    const image_io_plan &p
  ) {
    return image_io_create_info( e, resource, p );
  }
  auto get_image_io( const image_io_create_info &v ) {
    auto desc = add_vertex( *graph );
    auto shared_v = std::make_shared< image_io_create_info >( v );
    (*graph)[ desc ].set_create_info( shared_v );
    return shader_graph_vertex( graph, desc, shared_v );
  }
  compiled_shader_graph operator()() {
    auto list = build_command_list();
    reset();
    return compiled_shader_graph( resource, std::move( list ) );
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
  shader_graph_command_list build_command_list();
  void bind(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    const image_allocate_info &ai
  );
  void reuse(
    std::vector< image_binding >::iterator iter,
    const graph_type::vertex_descriptor &v,
    const std::string &name
  );
  std::pair< bool, unsigned int > is_ready_to_execulte(
    const std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v
  );
  shader_graph_command_list run(
    std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v
  );
  void reset() {
    independent_vertex.clear();
    binding.clear();
    graph.reset( new graph_type() );
  }
  std::shared_ptr< scene_graph::scene_graph_resource > resource;
  std::shared_ptr< graph_type > graph;
  std::vector< image_binding > binding;
  std::vector< graph_type::vertex_descriptor > independent_vertex;
};

}

#endif

