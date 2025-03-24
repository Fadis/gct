#ifndef GCT_SHADER_GRAPH_HPP
#define GCT_SHADER_GRAPH_HPP

#include "gct/color.hpp"
#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/directed_graph.hpp>
#include <boost/property_map/property_map.hpp>
#include <nlohmann/json_fwd.hpp>
#include <gct/setter.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/barrier_config.hpp>
#include <gct/color.hpp>

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

struct image_fill_create_info {
  LIBGCT_SETTER( name )
  LIBGCT_SETTER( color )
  image_fill_create_info &set_output(
    const std::string &n,
    const image_allocate_info &desc
  ) {
    auto basic = desc.create_info.get_basic();
    basic.setUsage(
      basic.usage |
      vk::ImageUsageFlagBits::eTransferSrc |
      vk::ImageUsageFlagBits::eTransferDst |
      vk::ImageUsageFlagBits::eStorage |
      vk::ImageUsageFlagBits::eSampled
    );
    auto desc_ = desc;
    desc_.create_info.set_basic( basic );
    desc_.set_layout( vk::ImageLayout::eGeneral );
    output = desc_;
    name = n;
    return *this;
  }
  image_fill_create_info &set_output(
    const std::string &name,
    unsigned int width,
    unsigned int height
  ) {
    return set_output(
      name,
      gct::image_allocate_info()
        .set_create_info(
          gct::image_create_info_t()
            .set_basic(
              gct::basic_2d_image(
                width,
                height
              )
            )
        )
    );
  }
  image_fill_create_info &set_output(
    const image_allocate_info &desc
  ) {
    auto basic = desc.create_info.get_basic();
    basic.setUsage(
      basic.usage |
      vk::ImageUsageFlagBits::eTransferSrc |
      vk::ImageUsageFlagBits::eTransferDst |
      vk::ImageUsageFlagBits::eStorage |
      vk::ImageUsageFlagBits::eSampled
    );
    auto desc_ = desc;
    desc_.create_info.set_basic( basic );
    desc_.set_layout( vk::ImageLayout::eGeneral );
    output = desc_;
    return *this;
  }
  image_fill_create_info &set_output(
    unsigned int width,
    unsigned int height
  ) {
    return set_output(
      gct::image_allocate_info()
        .set_create_info(
          gct::image_create_info_t()
            .set_basic(
              gct::basic_2d_image(
                width,
                height
              )
            )
        )
    );
  }
  image_fill_create_info &set_output(
    const image_pool::image_descriptor &desc
  ) {
    output = desc;
    return *this;
  }
  std::string name = "default";
  std::variant< image_pool::image_descriptor, image_allocate_info > output;
  std::array< float, 4u > color = color::web::pink;
  bool independent = true;
};

void to_json( nlohmann::json&, const image_fill_create_info& );

struct shader_graph_vertex_type {
  LIBGCT_SETTER( create_info )
  LIBGCT_SETTER( fill_create_info )
  LIBGCT_SETTER( color )
  std::shared_ptr< image_io_create_info > create_info;
  std::shared_ptr< image_fill_create_info > fill_create_info;
  boost::default_color_type color = boost::white_color; 
};

using shader_graph_graph_type =  boost::adjacency_list<
  boost::listS,
  boost::listS,
  boost::directedS,
  shader_graph_vertex_type,
  shader_graph_subedge_list
>;

struct shader_graph_barrier {
  LIBGCT_SETTER( state )
  LIBGCT_SETTER( view )
  barrier_state state;
  shader_graph_barrier &add_view( const image_pool::image_descriptor &v ) {
    view.push_back( v );
    return *this;
  }
  std::vector< image_pool::image_descriptor > view;
};

void to_json( nlohmann::json &dest, const shader_graph_barrier& );

using shader_graph_command = std::variant<
  std::shared_ptr< image_io >,
  shader_graph_barrier,
  std::shared_ptr< image_fill_create_info >
>;

void to_json( nlohmann::json &dest, const shader_graph_command& );
std::string to_string( const shader_graph_command& );

using shader_graph_command_list = std::vector<
  shader_graph_command
>;

void to_json( nlohmann::json &dest, const shader_graph_command_list& );
std::string to_string( const shader_graph_command_list& );

class shader_graph_vertex {
public:
  using graph_type = shader_graph_graph_type;
  struct subresult_type {
    LIBGCT_SETTER( vertex_id )
    LIBGCT_SETTER( create_info )
    LIBGCT_SETTER( fill_create_info )
    LIBGCT_SETTER( name )
    subresult_type(
      const graph_type::vertex_descriptor &v,
      const std::shared_ptr< image_io_create_info > &ci,
      const std::string &n
    ) : vertex_id( v ), create_info( ci ), name( n ) {}
    subresult_type(
      const graph_type::vertex_descriptor &v,
      const std::shared_ptr< image_fill_create_info > &ci,
      const std::string &n
    ) : vertex_id( v ), fill_create_info( ci ), name( n ) {}
    graph_type::vertex_descriptor vertex_id;
    std::shared_ptr< image_io_create_info > create_info;
    std::shared_ptr< image_fill_create_info > fill_create_info;
    std::string name;
  };
  class result_type {
  public:
    result_type(
      const std::shared_ptr< graph_type > &g,
      graph_type::vertex_descriptor id,
      const std::shared_ptr< image_io_create_info > &ci
    ) : graph( g ), vertex_id( id ), create_info( ci ) {}
    result_type(
      const std::shared_ptr< graph_type > &g,
      graph_type::vertex_descriptor id,
      const std::shared_ptr< image_fill_create_info > &ci
    ) : graph( g ), vertex_id( id ), fill_create_info( ci ) {}
    subresult_type operator[]( const std::string name ) const;
    operator subresult_type() const;
  private:
    std::shared_ptr< graph_type > graph;
    graph_type::vertex_descriptor vertex_id;
    std::shared_ptr< image_io_create_info > create_info;
    std::shared_ptr< image_fill_create_info > fill_create_info;
  };
  shader_graph_vertex(
    const std::shared_ptr< graph_type > &g,
    graph_type::vertex_descriptor id,
    const std::shared_ptr< image_io_create_info > &ci
  ) : graph( g ), vertex_id( id ), create_info( ci ) {}
  shader_graph_vertex(
    const std::shared_ptr< graph_type > &g,
    graph_type::vertex_descriptor id,
    const std::shared_ptr< image_fill_create_info > &ci
  ) : graph( g ), vertex_id( id ), fill_create_info( ci ) {}
  result_type operator()(
    const std::unordered_map< std::string, subresult_type > &input
  );
  result_type operator()(
    const subresult_type &input
  );
  result_type operator()();
private:
  std::shared_ptr< graph_type > graph;
  graph_type::vertex_descriptor vertex_id;
  std::shared_ptr< image_io_create_info > create_info;
  std::shared_ptr< image_fill_create_info > fill_create_info;
  bool called = false;
};

enum class image_mode {
  writable_without_sync = 0,
  readable_after_sync = 1,
  readable_without_sync_writable_after_sync = 2,
  readable_after_sync_writable_after_sync = 3
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

enum class image_generator_type {
  transfer = 0,
  output = 1,
  inout = 2
};

void to_json( nlohmann::json &dest, const compiled_shader_graph& );
std::string to_string( const compiled_shader_graph& );

class shader_graph_builder {
public:
  using graph_type = shader_graph_graph_type;
private:
  struct image_binding {
    image_binding() : used_by( new std::vector< std::pair< graph_type::vertex_descriptor, std::string > >() ) {}
    LIBGCT_SETTER( allocate_info )
    LIBGCT_SETTER( view )
    LIBGCT_SETTER( used_by )
    LIBGCT_SETTER( shareable )
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
    bool shareable = true;
  };
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
  auto get_image_fill( const image_fill_create_info &v ) {
    auto desc = add_vertex( *graph );
    auto shared_v = std::make_shared< image_fill_create_info >( v );
    (*graph)[ desc ].set_fill_create_info( shared_v );
    return shader_graph_vertex( graph, desc, shared_v );
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
  std::pair< bool, unsigned int > is_ready_to_execulte(
    const std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v,
    image_to_texture_map &texture
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

