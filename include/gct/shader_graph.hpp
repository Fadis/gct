#ifndef GCT_SHADER_GRAPH_HPP
#define GCT_SHADER_GRAPH_HPP

#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/property_map/property_map.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/barrier_config.hpp>
#include <gct/image_pool.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/texture_pool.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/color.hpp>
#include <gct/shader_graph_subedge.hpp>
#include <gct/shader_graph_barrier.hpp>
#include <gct/shader_graph_vertex_command.hpp>
#include <gct/shader_graph_command.hpp>
#include <gct/image_mode.hpp>
#include <gct/image_generator_type.hpp>

namespace gct {

class compute;
class image_view_t;
class image_io;
class image_io_create_info;
class image_fill_create_info;
class image_blit_create_info;
namespace scene_graph {
  class scene_graph_resource;
}

struct shader_graph_vertex_type {
  LIBGCT_SETTER( command )
  LIBGCT_SETTER( color )
  shader_graph_vertex_command command;
  boost::default_color_type color = boost::white_color; 
  std::string get_node_name() const;
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
    LIBGCT_SETTER( command )
    LIBGCT_SETTER( name )
    subresult_type(
      const std::shared_ptr< graph_type > &g,
      const graph_type::vertex_descriptor &v,
      const shader_graph_vertex_command &ci,
      const std::string &n
    ) : graph( g ), vertex_id( v ), command( ci ), name( n ) {}
    subresult_type(
      const image_pool::image_descriptor &im
    ) : command( im ) {}
    std::string get_node_name() const;
    std::shared_ptr< graph_type > graph;
    graph_type::vertex_descriptor vertex_id;
    shader_graph_vertex_command command;
    std::string name;
  };
  class result_type {
  public:
    result_type(
      const std::shared_ptr< graph_type > &g,
      graph_type::vertex_descriptor id,
      const shader_graph_vertex_command &ci
    ) : graph( g ), vertex_id( id ), command( ci ) {}
    subresult_type operator[]( const std::string name ) const;
    std::string get_node_name() const;
    operator subresult_type() const;
  private:
    std::shared_ptr< graph_type > graph;
    graph_type::vertex_descriptor vertex_id;
    shader_graph_vertex_command command;
  };
  class combined_result_type {
  public:
    combined_result_type &add(
      const std::string &n,
      const subresult_type &r
    ) {
      input.insert( std::make_pair( n, r ) );
      return *this;
    }
    subresult_type operator[]( const std::string name ) const;
    operator subresult_type() const;
    const std::unordered_map< std::string, subresult_type > &get() const {
      return input;
    }
  private:
    std::unordered_map< std::string, subresult_type > input;
  };
  shader_graph_vertex(
    const std::shared_ptr< scene_graph::scene_graph_resource > &r,
    const std::shared_ptr< graph_type > &g,
    graph_type::vertex_descriptor id,
    const shader_graph_vertex_command &ci
  ) : resource( r ), graph( g ), vertex_id( id ), command( ci ) {}
  result_type operator()(
    const std::unordered_map< std::string, subresult_type > &input
  );
  result_type operator()(
    const combined_result_type &input
  );
  result_type operator()(
    const subresult_type &input
  );
  result_type operator()();
  std::string get_node_name() const;
private:
  std::shared_ptr< scene_graph::scene_graph_resource > resource;
  std::shared_ptr< graph_type > graph;
  graph_type::vertex_descriptor vertex_id;
  shader_graph_vertex_command command;
  bool called = false;
};


struct shader_graph_image_binding {
  shader_graph_image_binding() : used_by( new std::vector< std::pair< shader_graph_graph_type::vertex_descriptor, std::string > >() ) {}
  LIBGCT_SETTER( allocate_info )
  LIBGCT_SETTER( view )
  LIBGCT_SETTER( used_by )
  LIBGCT_SETTER( shareable )
  shader_graph_image_binding &add_used_by(
    const shader_graph_graph_type::vertex_descriptor &v,
    const std::string &n
  ) {
    used_by->emplace_back( v, n );
    return *this;
  }
  std::shared_ptr< image_allocate_info > allocate_info;
  image_pool::image_descriptor view;
  std::shared_ptr< std::vector< std::pair< shader_graph_graph_type::vertex_descriptor, std::string > > > used_by;
  bool shareable = true;
};

class compiled_shader_graph {
private:
  using image_binding = shader_graph_image_binding;
public:
  using graph_type = shader_graph_graph_type;
  compiled_shader_graph(
    const std::shared_ptr< scene_graph::scene_graph_resource > &r,
    const std::shared_ptr< graph_type > &g,
    std::vector< image_binding > &&b,
    shader_graph_command_list &&l
  ) : resource( r ), graph( g ), list( l ), binding( std::move( b ) ) {}
  void operator()(
    command_buffer_recorder_t &rec
  ) const;
  const shader_graph_command_list &get_list() const {
    return list;
  }
  const std::vector< image_binding > &get_binding() const {
    return binding;
  }
  const graph_type &get_graph() const {
    return *graph;
  }
  std::shared_ptr< image_view_t > get_view( const shader_graph_vertex::subresult_type & ) const;
private:
  std::shared_ptr< scene_graph::scene_graph_resource > resource;
  std::shared_ptr< graph_type > graph;
  shader_graph_command_list list;
  std::vector< image_binding > binding;
};

void to_json( nlohmann::json &dest, const compiled_shader_graph& );
std::string to_string( const compiled_shader_graph& );

class shader_graph_builder {
public:
  using graph_type = shader_graph_graph_type;
private:
  using image_binding = shader_graph_image_binding;
  struct image_state {
    image_state(
      const image_binding &b
    ) : generators( b.used_by ) {}
    LIBGCT_SETTER( mode )
    LIBGCT_SETTER( expected_consumer )
    LIBGCT_SETTER( next_expected_consumer )
    LIBGCT_SETTER( generators )
    LIBGCT_SETTER( next_generator )
    LIBGCT_SETTER( last_generator_type )
    image_mode mode = image_mode::writable_without_sync;
    std::vector< graph_type::vertex_descriptor > expected_consumer;
    std::vector< graph_type::vertex_descriptor > next_expected_consumer;
    std::shared_ptr< std::vector< std::pair< graph_type::vertex_descriptor, std::string > > > generators;
    int next_generator = 0;
    image_generator_type last_generator_type = image_generator_type::transfer;
  };
  using image_state_map = std::unordered_map<
    image_pool::image_descriptor,
    image_state
  >;
  struct subresult_hash {
    [[nodiscard]] std::size_t operator()( const std::pair< graph_type::vertex_descriptor, std::string > & ) const;
  };
  struct texture_hash {
    [[nodiscard]] std::size_t operator()( const std::pair< image_pool::image_descriptor, sampler_pool::sampler_descriptor > & ) const;
  };
  using image_to_texture_map = std::unordered_map< std::pair< image_pool::image_descriptor, sampler_pool::sampler_descriptor >, texture_pool::texture_descriptor, texture_hash >;
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
  shader_graph_vertex fill( const image_fill_create_info &v );
  shader_graph_vertex fill( std::uint32_t width, std::uint32_t height = 1u, const std::array< float, 4u > &color = color::special::transparent );
  shader_graph_vertex blit( const image_blit_create_info &v );
  shader_graph_vertex call( const image_io_create_info &v );
  shader_graph_vertex call(
    const std::shared_ptr< compute > &e,
    const image_io_plan &p
  );
  void output( const shader_graph_vertex::subresult_type & );
  compiled_shader_graph operator()() {
    auto list = build_command_list();
    const auto compiled = compiled_shader_graph( resource, graph, std::move( binding ), std::move( list ) );
    reset();
    return compiled;
  }
private:
  std::vector< std::pair< graph_type::vertex_descriptor, std::string > > get_consumer_of(
    const graph_type::vertex_descriptor &, const std::string&, bool include_middle
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
  void assign_image(
    image_to_texture_map &texture
  );
  shader_graph_command_list build_command_list();
  void set_image_on_consumer(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    image_to_texture_map &texture
  );
  void bind(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    const image_allocate_info &ai,
    bool shareable,
    image_to_texture_map &texture
  );
  void reuse(
    std::vector< image_binding >::iterator iter,
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    image_to_texture_map &texture
  );
  std::pair< bool, unsigned int > is_ready_to_execute(
    const std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v,
    const image_to_texture_map &texture
  );
  shader_graph_command_list run(
    std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v,
    const image_to_texture_map &texture
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

