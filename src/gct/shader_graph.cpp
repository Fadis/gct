#include <limits>
#include <queue>
#include <string>
#include <utility>
#include <nlohmann/json.hpp>
#include <vulkan2json/ImageUsageFlags.hpp>
#include <vulkan2json/Format.hpp>
#include <vulkan2json/ImageLayout.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/properties.hpp>
#include <boost/range/iterator_range.hpp>
#include <gct/exception.hpp>
#include <gct/compute.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/image_io.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/image_fill_create_info.hpp>
#include <gct/image_blit_create_info.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/shader_graph.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const compiled_shader_graph &src ) {
  dest = src.get_list();
}

std::string to_string( const compiled_shader_graph &src ) {
  std::string serialized;
  for( const auto &b: src.get_binding() ) {
    serialized += "Image " + std::to_string( *b.view ) + "\n";
    serialized += "  extent : ";
    serialized += std::to_string( b.allocate_info->create_info.get_basic().extent.width );
    serialized += "x";
    serialized += std::to_string( b.allocate_info->create_info.get_basic().extent.height );
    serialized += "x";
    serialized += std::to_string( b.allocate_info->create_info.get_basic().extent.depth );
    serialized += " layers : ";
    if( b.allocate_info->range ) {
      serialized += std::to_string( b.allocate_info->range->layer_offset );
      serialized += " to ";
      serialized += std::to_string( b.allocate_info->range->layer_offset + b.allocate_info->range->layer_count - 1u );
    }
    else {
      serialized += "0 to ";
      serialized += std::to_string( b.allocate_info->create_info.get_basic().arrayLayers - 1u );
    }
    serialized += " mip : ";
    if( b.allocate_info->range ) {
      serialized += std::to_string( b.allocate_info->range->mip_offset );
      serialized += " to ";
      serialized += std::to_string( b.allocate_info->range->mip_offset + b.allocate_info->range->mip_count - 1u );
    }
    else {
      serialized += "0 to ";
      serialized += std::to_string( b.allocate_info->create_info.get_basic().mipLevels - 1u );
    }
    if( b.shareable ) {
      serialized += " shared";
    }
    serialized += "\n";
    serialized += "  usage : " + nlohmann::json( b.allocate_info->create_info.get_basic().usage ).dump() + "\n";
    serialized += "  format : " + nlohmann::json( b.allocate_info->create_info.get_basic().format ).dump() + "\n";
    if( b.allocate_info->layout ) {
      serialized += "  layout : " + nlohmann::json( *b.allocate_info->layout ).dump() + "\n";
    }
    serialized += "  used by : \n";
    for( const auto &u: *b.used_by ) {
      serialized += "    " + src.get_graph()[ u.first ].get_node_name() + "." + u.second + "\n";
    }
  }
  serialized += "\n";
  serialized += "Graph\n";
  const auto [v_begin,v_end] = vertices( src.get_graph() );
  for( const auto &v: boost::make_iterator_range( v_begin, v_end ) ) {
    const auto [out_begin,out_end] = out_edges( v, src.get_graph() );
    for( const auto &e: boost::make_iterator_range( out_begin, out_end ) ) {
      const auto t = target( e, src.get_graph() );
      serialized += "  " + src.get_graph()[ v ].get_node_name() + " -> " + src.get_graph()[ t ].get_node_name() + "\n";
    }
  }
  serialized += "\n";
  serialized += to_string( src.get_list() );

  return serialized;
}



  shader_graph_vertex::subresult_type shader_graph_vertex::result_type::operator[]( const std::string name ) const {
    if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
      if( create_info->get_plan().output.find( name ) == create_info->get_plan().output.end() ) {
        if( create_info->get_plan().inout.find( name ) == create_info->get_plan().inout.end() ) {
          throw exception::invalid_argument( "shader_graph_vertex::result_type::operator[] : Output image " + name + "does not exist", __FILE__, __LINE__ );
        }
      }
      return shader_graph_vertex::subresult_type(
        graph,
        vertex_id,
        command,
        name
      );
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &fill_create_info = std::get< std::shared_ptr< image_fill_create_info > >( command );
      if( name != fill_create_info->name ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator[] : Output image " + name + "does not exist", __FILE__, __LINE__ );
      }
      return shader_graph_vertex::subresult_type(
        graph,
        vertex_id,
        command,
        name
      );
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
      if( name != create_info->output_name ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator[] : Output image " + name + "does not exist", __FILE__, __LINE__ );
      }
      return shader_graph_vertex::subresult_type(
        graph,
        vertex_id,
        command,
        name
      );
    }
    else {
      throw exception::runtime_error( "shader_graph_vertex::result_type::operator[] : Broken result_type", __FILE__, __LINE__ );
    }
  }
  shader_graph_vertex::result_type:: operator subresult_type() const {
    if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
      if( create_info->get_plan().output.size() == 1u && create_info->get_plan().inout.size() == 0u ) {
        return shader_graph_vertex::subresult_type(
          graph,
          vertex_id,
          command,
          create_info->get_plan().output.begin()->first
        );
      }
      else if( create_info->get_plan().inout.size() == 1u && create_info->get_plan().output.size() == 0u ) {
        return shader_graph_vertex::subresult_type(
          graph,
          vertex_id,
          command,
          *create_info->get_plan().inout.begin()
        );
      }
      else {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator subresult_type : result_type has multiple output images", __FILE__, __LINE__ );
      }
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( command );
      return shader_graph_vertex::subresult_type(
        graph,
        vertex_id,
        command,
        create_info->name
      );
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
      return shader_graph_vertex::subresult_type(
        graph,
        vertex_id,
        command,
        create_info->output_name
      );
    }
    else {
      throw exception::invalid_argument( "shader_graph_vertex::result_type::operator subresult_type : broken result_type", __FILE__, __LINE__ );
    }
  }
  shader_graph_vertex::subresult_type shader_graph_vertex::combined_result_type::operator[]( const std::string name ) const {
    const auto match = input.find( name );
    if( match == input.end() ) {
      throw exception::invalid_argument( "shader_graph_vertex::combined_result_type::operator[] : Unknown subresult name "+name, __FILE__, __LINE__ );
    }
    return match->second;
  }
  shader_graph_vertex::combined_result_type:: operator subresult_type() const {
    if( input.size() != 1u ) {
      throw exception::invalid_argument( "shader_graph_vertex::combined_result_type::operator subresult_type : result_type has multiple output images", __FILE__, __LINE__ );
    }
    return input.begin()->second;
  }

  shader_graph_vertex::result_type shader_graph_vertex::operator()(
    const subresult_type &input
  ) {
    std::string name;
    if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
      if(
        create_info->get_plan().input.size() == 1u &&
        create_info->get_plan().sampled.size() == 0u &&
        create_info->get_plan().inout.size() == 0u ) {
        name = *create_info->get_plan().input.begin();
      }
      else if(
        create_info->get_plan().input.size() == 0u &&
        create_info->get_plan().sampled.size() == 1u &&
        create_info->get_plan().inout.size() == 0u ) {
        name = create_info->get_plan().sampled.begin()->first;
      }
      else if(
        create_info->get_plan().input.size() == 0u &&
        create_info->get_plan().sampled.size() == 0u &&
        create_info->get_plan().inout.size() == 1u ) {
        name = *create_info->get_plan().inout.begin();
      }
      else {
        throw exception::invalid_argument( "shader_graph_vertex::operator() : vertex has multiple input images", __FILE__, __LINE__ );
      }
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
      throw exception::invalid_argument( "shader_graph_vertex::operator() : vertex has no input images", __FILE__, __LINE__ );
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
      name = create_info->input_name;
    }
    else {
      throw exception::invalid_argument( "shader_graph_vertex::operator() : broken vertex", __FILE__, __LINE__ );
    }
    return (*this)( combined_result_type().add( name, input ) );
  }
  shader_graph_vertex::result_type shader_graph_vertex::operator()() {
    return (*this)( combined_result_type() );
  }
  shader_graph_vertex::result_type shader_graph_vertex::operator()(
    const combined_result_type &input
  ) {
    return (*this)( input.get() );
  }
  shader_graph_vertex::result_type shader_graph_vertex::operator()(
    const std::unordered_map< std::string, subresult_type > &input
  ) {
    if( called ) {
      throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Vertex must not be called multiple times", __FILE__, __LINE__ );
    }
    std::unordered_set< graph_type::vertex_descriptor > incoming_fill;
    if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
      for( const auto &i: input ) {
        if( create_info->get_plan().input.find( i.first ) == create_info->get_plan().input.end() ) {
          if( create_info->get_plan().inout.find( i.first ) == create_info->get_plan().inout.end() ) {
            if( create_info->get_plan().sampled.find( i.first ) == create_info->get_plan().sampled.end() ) {
              throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Input image " + i.first + "does not exist", __FILE__, __LINE__ );
            }
          }
        }
        if( shader_graph_vertex_command_id( i.second.command.index() ) == shader_graph_vertex_command_id::fill ) {
          incoming_fill.insert( i.second.vertex_id );
        }
      }
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( command );
      if( !input.empty() ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Fill does not take any inputs", __FILE__, __LINE__ );
      }
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
      if( input.size() != 1 ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Blit take only one input", __FILE__, __LINE__ );
      }
      if( create_info->input_name != input.begin()->first ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Input image " + input.begin()->first + "does not exist", __FILE__, __LINE__ );
      }
    }
    else {
      throw exception::runtime_error( "shader_graph_vertex::result_type::operator() : Broken vertex", __FILE__, __LINE__ );
    }
    std::unordered_set< std::string > immediate;
    for( const auto &i: input ) {
      if( shader_graph_vertex_command_id( i.second.command.index() ) == shader_graph_vertex_command_id::immediate ) {
        immediate.insert( i.first );
        const auto &view = std::get< image_pool::image_descriptor >( i.second.command );
        if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
          const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
          const auto sampler = create_info->get_plan().sampled.find( i.first );
          if( sampler != create_info->get_plan().sampled.end() ) {
            const auto tex = resource->texture->allocate(
              sampler->second,
              view
            );
            create_info->add( i.first, tex );
          }
          else {
            create_info->add( i.first, view );
          }
        }
        else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
          throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Fill does not take any inputs", __FILE__, __LINE__ );
        }
        else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
          const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
          if( create_info->input_name == i.first ) {
            create_info->input = view;
          }
        }
        else {
          throw exception::runtime_error( "shader_graph_vertex::result_type::operator() : Broken vertex", __FILE__, __LINE__ );
        }
      }
    }
    auto input_ = input;
    for( const auto &i: immediate ) {
      input_.erase( i );
    }
    std::unordered_map< graph_type::vertex_descriptor, graph_type::edge_descriptor > unique_edge;
    for( const auto &i: input_ ) {
      if( i.second.vertex_id != vertex_id ) {
        if( unique_edge.find( i.second.vertex_id ) == unique_edge.end() ) {
          auto [desc,inserted] = add_edge( i.second.vertex_id, vertex_id, *graph );
          if( !inserted ) {
            throw exception::runtime_error( "shader_graph_vertex::result_type::operator() : Edge insertion from " + i.second.name + " to " + i.first + " failed.", __FILE__, __LINE__ );
          }
          for( const auto &f : incoming_fill ) {
            auto [desc,inserted] = add_edge( i.second.vertex_id, f, *graph );
            std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ f ].command )->independent = false;
          }
          unique_edge.insert( std::make_pair( i.second.vertex_id, desc ) );
        }
      }
    }
    for( const auto &i: input_ ) {
      (*graph)[ unique_edge[ i.second.vertex_id ] ].push_back(
        shader_graph_subedge()
          .set_from( i.second.name )
          .set_to( i.first )
      );
    }
    called = true;
    return result_type(
      graph,
      vertex_id,
      command
    );
  }
  std::string shader_graph_vertex_type::get_node_name() const {
    std::string name;
    if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
      name = create_info->get_plan().node_name;
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( command );
      name = create_info->node_name;
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
      name = create_info->node_name;
    }
    else {
      throw exception::invalid_argument( "compiled_shader_graph::get_node_name : broken vertex", __FILE__, __LINE__ );
    }
    if( name.empty() ) {
      name = std::to_string( std::intptr_t( this ) );
    }
    return name;
  }
  std::string shader_graph_vertex::subresult_type::get_node_name() const {
    return (*graph)[ vertex_id ].get_node_name() + ".result." + name;
  }
  std::string shader_graph_vertex::result_type::get_node_name() const {
    return (*graph)[ vertex_id ].get_node_name() + ".result";
  }
  std::string shader_graph_vertex::get_node_name() const {
    std::string name;
    if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( command );
      name = create_info->get_plan().node_name;
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( command );
      name = create_info->node_name;
    }
    else if( shader_graph_vertex_command_id( command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( command );
      name = create_info->node_name;
    }
    else {
      throw exception::invalid_argument( "compiled_shader_graph::get_node_name : broken vertex", __FILE__, __LINE__ );
    }
    if( name.empty() ) {
      name = std::to_string( std::intptr_t( this ) );
    }
    return name;
  }
  std::size_t shader_graph_builder::subresult_hash::operator()( const std::pair< graph_type::vertex_descriptor, std::string > &v ) const {
    return std::hash< void* >()( v.first ) ^ std::hash< std::string >()( v.second );
  }
  std::size_t shader_graph_builder::texture_hash::operator()( const std::pair< image_pool::image_descriptor, sampler_pool::sampler_descriptor > &v ) const {
    return
      std::hash< std::size_t >()( v.first ? *v.first : std::numeric_limits< std::size_t >::max() ) ^
      std::hash< std::size_t >()( v.second ? ( *v.second * 65536u ) : std::numeric_limits< std::size_t >::max() );
  }
  std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > >
  shader_graph_builder::get_consumer_of(
    const graph_type::vertex_descriptor &generator,
    const std::string &name,
    bool include_middle
  ) const {
    std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > > temp;
    std::unordered_set< std::pair< graph_type::vertex_descriptor, std::string >, subresult_hash > next_depth{ std::make_pair( generator, name ) };
    std::unordered_set< std::pair< graph_type::vertex_descriptor, std::string >, subresult_hash > current_depth;
    std::unordered_set< std::pair< graph_type::vertex_descriptor, std::string >, subresult_hash > consumer;
    std::unordered_set< std::pair< graph_type::vertex_descriptor, std::string >, subresult_hash > consumed;
    do {
      current_depth = std::move( next_depth );
      next_depth.clear();
      consumed.clear();
      for( const auto sub: current_depth ) {
        const auto [out_begin,out_end] = out_edges( sub.first, *graph );
        for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
          const auto connected = std::find_if(
            (*graph)[ edge ].begin(),
            (*graph)[ edge ].end(),
            [&]( const auto &e ) { return e.from == sub.second; }
          );
          if( connected != (*graph)[ edge ].end() ) {
            const auto dest = target( edge, *graph );
            if( shader_graph_vertex_command_id( (*graph)[ dest ].command.index() ) == shader_graph_vertex_command_id::call ) {
              const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ dest ].command );
              const auto inout = std::find_if(
                create_info->get_plan().inout.begin(),
                create_info->get_plan().inout.end(),
                [&]( const auto &i ) {
                  return i == connected->to;
                }
              );
              if( inout != create_info->get_plan().inout.end() ) {
                next_depth.insert( std::make_pair( dest, connected->to ) );
              }
              else {
                const auto in = std::find_if(
                  create_info->get_plan().input.begin(),
                  create_info->get_plan().input.end(),
                  [&]( const auto &i ) {
                    return i == connected->to;
                  }
                );
                if( in != create_info->get_plan().input.end() ) {
                  consumer.insert( std::make_pair( dest, connected->to ) );
                  consumed.insert( sub );
                }
                else {
                  const auto sampled = std::find_if(
                    create_info->get_plan().sampled.begin(),
                    create_info->get_plan().sampled.end(),
                    [&]( const auto &i ) {
                      return i.first == connected->to;
                    }
                  );
                  if( sampled != create_info->get_plan().sampled.end() ) {
                    consumer.insert( std::make_pair( dest, connected->to ) );
                    consumed.insert( sub );
                  }
                }
              }
            }
            else if( shader_graph_vertex_command_id( (*graph)[ dest ].command.index() ) == shader_graph_vertex_command_id::fill ) {
            }
            else if( shader_graph_vertex_command_id( (*graph)[ dest ].command.index() ) == shader_graph_vertex_command_id::blit ) {
              const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ dest ].command );
              if( create_info->input_name == connected->to ) {
                next_depth.insert( std::make_pair( dest, connected->to ) );
              }
            }
            else {
              throw -1;
            }
          }
        }
      }
      if( include_middle ) {
        consumer.insert( current_depth.begin(), current_depth.end() );
      }
      for( const auto &sub: consumed ) {
        current_depth.erase( sub );
      }
    } while( !next_depth.empty() );
    if( include_middle ) {
      consumer.insert( current_depth.begin(), current_depth.end() );
    }
    consumer.erase( std::make_pair( generator, name ) );
    return std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > >(
      consumer.begin(),
      consumer.end()
    );
  }

  shader_graph_vertex shader_graph_builder::fill( const image_fill_create_info &v ) {
    auto desc = add_vertex( *graph );
    auto shared_v = std::make_shared< image_fill_create_info >( v );
    (*graph)[ desc ].set_command( shared_v );
    return shader_graph_vertex( resource, graph, desc, shared_v );
  }
  shader_graph_vertex shader_graph_builder::fill( std::uint32_t width, std::uint32_t height, const std::array< float, 4u > &color ) {
    return fill(
      gct::image_fill_create_info()
        .set_output( width, height )
        .set_color( color )
    );
  }
  shader_graph_vertex shader_graph_builder::blit( const image_blit_create_info &v ) {
    auto desc = add_vertex( *graph );
    auto shared_v = std::make_shared< image_blit_create_info >( v );
    (*graph)[ desc ].set_command( shared_v );
    return shader_graph_vertex( resource, graph, desc, shared_v );
  }
  shader_graph_vertex shader_graph_builder::call( const image_io_create_info &v ) {
    auto desc = add_vertex( *graph );
    auto shared_v = std::make_shared< image_io_create_info >( v );
    (*graph)[ desc ].set_command( shared_v );
    return shader_graph_vertex( resource, graph, desc, shared_v );
  }
  shader_graph_vertex shader_graph_builder::call(
    const std::shared_ptr< compute > &e,
    const image_io_plan &p
  ) {
    return call( get_image_io_create_info( e, p ) );
  }
  bool shader_graph_builder::shareable(
    const graph_type::vertex_descriptor &first_generator,
    const std::string &first_subedge,
    const graph_type::vertex_descriptor &second_generator
  ) const {
    const auto &vertex = (*graph)[ first_generator ];
    unsigned int id = 0u;
    if( shader_graph_vertex_command_id( vertex.command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( vertex.command );
      const auto &out = create_info->get_plan().output.find( first_subedge );
      if( out != create_info->get_plan().output.end() ) {
        if( out->second.index() == 0 ) {
          const auto view = std::get< image_pool::image_descriptor >( out->second );
          id = *view;
          const auto b = std::find_if( 
            binding.begin(),
            binding.end(),
            [&]( const auto &b ) -> bool {
              return b.view == view;
            }
          );
          if( b != binding.end() ) {
            if( !b->shareable ) {
              return false;
            }
          }
          else {
            throw exception::runtime_error( "shader_graph_builder::shareable : Corrpted binding : An image generated by generator is not recorded on binding", __FILE__, __LINE__ );
          }
        }
        else {
          throw exception::invalid_argument( "shader_graph_builder::shareable : Output image of source generator must be bound prior", __FILE__, __LINE__ );
        }
      }
      else {
        throw exception::invalid_argument( "shader_graph_builder::shareable : Source generator does not have an output image named " + first_subedge, __FILE__, __LINE__ );
      }
    }
    else if( shader_graph_vertex_command_id( vertex.command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( vertex.command );
      const auto &out = create_info->output;
      if( create_info->name == first_subedge ) {
        if( out.index() == 0 ) {
          const auto view = std::get< image_pool::image_descriptor >( out );
          id = *view;
          const auto b = std::find_if( 
            binding.begin(),
            binding.end(),
            [&]( const auto &b ) -> bool {
              return b.view == view;
            }
          );
          if( b != binding.end() ) {
            if( !b->shareable ) {
              return false;
            }
          }
          else {
            throw exception::runtime_error( "shader_graph_builder::shareable : Corrpted binding : An image generated by generator is not recorded on binding", __FILE__, __LINE__ );
          }
        }
        else {
          throw exception::invalid_argument( "shader_graph_builder::shareable : Output image of source generator must be bound prior", __FILE__, __LINE__ );
        }
      }
      else {
        throw exception::invalid_argument( "shader_graph_builder::shareable : Source generator does not have an output image named " + first_subedge, __FILE__, __LINE__ );
      }
    }
    else if( shader_graph_vertex_command_id( vertex.command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( vertex.command );
      const auto &out = create_info->output;
      if( create_info->output_name == first_subedge ) {
        if( out.index() == 0 ) {
          const auto view = std::get< image_pool::image_descriptor >( out );
          id = *view;
          const auto b = std::find_if( 
            binding.begin(),
            binding.end(),
            [&]( const auto &b ) -> bool {
              return b.view == view;
            }
          );
          if( b != binding.end() ) {
            if( !b->shareable ) {
              return false;
            }
          }
          else {
            throw exception::runtime_error( "shader_graph_builder::shareable : Corrpted binding : An image generated by generator is not recorded on binding", __FILE__, __LINE__ );
          }
        }
        else {
          throw exception::invalid_argument( "shader_graph_builder::shareable : Output image of source generator must be bound prior", __FILE__, __LINE__ );
        }
      }
      else {
        throw exception::invalid_argument( "shader_graph_builder::shareable : Source generator does not have an output image named " + first_subedge, __FILE__, __LINE__ );
      }
    }
    else {
      throw exception::invalid_argument( "shader_graph_builder::shareable : Corrupted vertex", __FILE__, __LINE__ );
    }
    bool has_consumer = false;
    for( const auto &v: get_consumer_of( first_generator, first_subedge, false ) ) {
      has_consumer = true;
      if( v.first == second_generator ) {
        return false;
      }
      const auto [v_begin,v_end] = vertices( *graph );
      for( const auto &v: boost::make_iterator_range( v_begin, v_end ) ) {
        (*graph)[ v ].color = boost::white_color;
      }
      if( !boost::is_reachable( v.first, second_generator, *graph, boost::get( &shader_graph_vertex_type::color, *graph ) ) ) {
        return false;
      }
    }
    if( !has_consumer ) {
      return false;
    }
    return true;
  }
  bool shader_graph_builder::shareable(
    const std::vector< std::pair< graph_type::vertex_descriptor, std::string > > &first_generator,
    const graph_type::vertex_descriptor &second_generator
  ) const {
    for( const auto &f: first_generator ) {
      if( !shareable( f.first, f.second, second_generator ) ) {
        return false;
      }
    }
    return true;
  }
  void shader_graph_builder::reuse(
    std::vector< image_binding >::iterator iter,
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    image_to_texture_map &texture
  ) {
    iter->add_used_by( v, name );
    if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
      std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command )->add( name, iter->view );
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ v ].command );
      if( create_info->name == name ) {
        create_info->output = iter->view;
      }
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
      if( create_info->input_name == name ) {
        create_info->input = iter->view;
        create_info->independent = false;
      }
      else if( create_info->output_name == name ) {
        create_info->output = iter->view;
      }
    }
    set_image_on_consumer( v, name, iter->view, texture );
  }
  void shader_graph_builder::set_image_on_consumer(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    image_to_texture_map &texture
  ) {
    for( const auto &c: get_consumer_of( v, name, true ) ) {
      if( shader_graph_vertex_command_id( (*graph)[ c.first ].command.index() ) == shader_graph_vertex_command_id::call ) {
        const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ c.first ].command );
        const auto tex_config = create_info->get_plan().sampled.find( c.second );
          create_info->get_plan().sampled.end();
        if( tex_config != create_info->get_plan().sampled.end() ) {
          const auto existing = texture.find( std::make_pair( view, tex_config->second ) );
          if( existing != texture.end() ) {
            create_info->add( c.second, existing->second );
          }
          else {
            const auto tex = resource->texture->allocate(
              tex_config->second,
              view
            );
            texture.insert( std::make_pair( std::make_pair( view, tex_config->second ), tex ) );
            create_info->add( c.second, tex );
          }
        }
        else {
          create_info->add( c.second, view );
        }
      }
      else if( shader_graph_vertex_command_id( (*graph)[ c.first ].command.index() ) == shader_graph_vertex_command_id::fill ) {
      }
      else if( shader_graph_vertex_command_id( (*graph)[ c.first ].command.index() ) == shader_graph_vertex_command_id::blit ) {
        const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ c.first ].command );
        if( c.second == create_info->input_name ) {
          create_info->input = view;
          create_info->independent = false;
        }
      }
      else {
        throw exception::invalid_argument( "shader_graph_builder::set_image_on_consumer : Corrupted vertex", __FILE__, __LINE__ );
      }
    }
  }
  void shader_graph_builder::bind(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    const image_allocate_info &ai,
    bool shareable,
    image_to_texture_map &texture
  ) {
    binding.push_back(
      image_binding()
        .set_allocate_info( std::make_shared< image_allocate_info >( ai ) )
        .set_view( view )
        .add_used_by( v, name )
        .set_shareable( shareable )
    );
    if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command );
      if( !create_info->is_ready( name ) ) {
        create_info->add( name, view );
      }
      else {
        const auto existing = create_info->get( name );
        if( existing.index() == 0 ) {
          if( !std::get< image_pool::image_descriptor >( existing ) ) {
            throw exception::runtime_error( "shader_graph_builder::bind : unable to get bound image view", __FILE__, __LINE__ );
          }
          if( std::get< image_pool::image_descriptor >( existing ) != view ) {
            throw exception::runtime_error( "shader_graph_builder::bind : inconsistent image binding", __FILE__, __LINE__ );
          }
        }
        else {
          throw exception::runtime_error( "shader_graph_builder::bind : uninplemented", __FILE__, __LINE__ );
        }
      }
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ v ].command );
      if( create_info->name != name ) {
        throw exception::runtime_error( "shader_graph_builder::bind : unknown resource name " + name, __FILE__, __LINE__ );
      }
      if( create_info->output.index() == 1 ) {
        create_info->output = view;
      }
      else if( create_info->output.index() == 2 ) {
        create_info->output = view;
      }
      else if( create_info->output.index() == 0 ) {
        const auto existing = std::get< image_pool::image_descriptor >( create_info->output );
        if( !existing ) {
          throw exception::runtime_error( "shader_graph_builder::bind : unable to get bound image view", __FILE__, __LINE__ );
        }
        if( existing != view ) {
          throw exception::runtime_error( "shader_graph_builder::bind : inconsistent image binding", __FILE__, __LINE__ );
        }
      }
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
      if( create_info->input_name != name || create_info->output_name != name ) {
        throw exception::runtime_error( "shader_graph_builder::bind : unknown resource name " + name, __FILE__, __LINE__ );
      }
      if( create_info->output_name == name ) {
        if( create_info->output.index() == 1 ) {
          create_info->output = view;
        }
        if( create_info->output.index() == 2 ) {
          create_info->output = view;
        }
        else if( create_info->output.index() == 0 ) {
          const auto existing = std::get< image_pool::image_descriptor >( create_info->output );
          if( !existing ) {
            throw exception::runtime_error( "shader_graph_builder::bind : unable to get bound image view", __FILE__, __LINE__ );
          }
          if( existing != view ) {
            throw exception::runtime_error( "shader_graph_builder::bind : inconsistent image binding", __FILE__, __LINE__ );
          }
        }
      }
      else if( create_info->input_name == name ) {
          create_info->input = view;
          create_info->independent = false;
      }
    }
    set_image_on_consumer( v, name, view, texture );
  }
  void shader_graph_builder::assign_image(
    image_to_texture_map &texture
  ) {
    const auto [v_begin,v_end] = vertices( *graph );
    std::queue< graph_type::vertex_descriptor > v_cur;
    std::unordered_set< graph_type::vertex_descriptor > visited;
    for( const auto &v: boost::make_iterator_range( v_begin, v_end ) ) {
      if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
        const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command );
        if( create_info->independent() ) {
          v_cur.push( v );
          independent_vertex.push_back( v );
          visited.insert( v );
        }
      }
      else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
        const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ v ].command );
        if( create_info->independent ) {
          v_cur.push( v );
          independent_vertex.push_back( v );
          visited.insert( v );
        }
      }
      else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
        const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
        if( create_info->independent ) {
          v_cur.push( v );
          independent_vertex.push_back( v );
          visited.insert( v );
        }
      }
      else {
        throw exception::invalid_argument( "shader_graph_builder::assign_image : Corrupted vertex", __FILE__, __LINE__ );
      }
    }
    if( v_cur.empty() ) {
      throw exception::runtime_error( "shader_graph_builder::assign_image : At least one vertex must be independent", __FILE__, __LINE__ );
    }
    while( !v_cur.empty() ) {
      const auto &v = v_cur.front();
      if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
        const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command );
        for( const auto &inout: create_info->get_inout() ) {
          const std::string &name = inout.first;
          const auto &desc = inout.second;
          const auto view = resource->image->get( desc );
          bind( v, name, desc,
            image_allocate_info()
              .set_create_info( view->get_factory()->get_props() )
              .set_range(
                subview_range()
                  .set_mip_offset( view->get_props().get_basic().subresourceRange.baseMipLevel )
                  .set_mip_count( view->get_props().get_basic().subresourceRange.levelCount )
                  .set_layer_offset( view->get_props().get_basic().subresourceRange.baseArrayLayer )
                  .set_layer_count( view->get_props().get_basic().subresourceRange.layerCount )
              ),
            false,
            texture
          );
        }
        for( const auto &out: create_info->get_plan().output ) {
          const std::string &name = out.first;
          if( out.second.index() == 1u ) {
            const auto &ai = std::get< image_allocate_info >( out.second );
            bool solved = false;
            for( auto cur = binding.begin(); cur != binding.end(); ++cur ) {
              cur = std::find_if(
                cur,
                binding.end(),
                [&]( const auto &b ) -> bool {
                  return *b.allocate_info == ai;
                }
              );
              if( cur == binding.end() ) {
                break;
              }
              else {
                if( shareable( *cur->used_by, v ) ) {
                  reuse( cur, v, name, texture );
                  cur->shareable = create_info->is_shareable( name );
                  solved = true;
                  break;
                }
              }
            }
            if( !solved ) {
              const auto view = resource->image->allocate( ai ).linear;
              const bool shareable = create_info->is_shareable( name );
              bind( v, name, view, ai, shareable, texture );
            }
          }
          if( out.second.index() == 2u ) {
            auto ai = std::get< dynamic_size_image_allocate_info >( out.second );
            glm::vec4 size( 0.0f, 0.0f, 0.0f, 1.0f );
            std::optional< vk::ImageType > image_type;
            std::optional< vk::Format > format;
            std::uint32_t layer_count = 1u;
            if( ai.dim.relative_to ) {
              if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
                const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command );
                const auto image_or_tex = create_info->get( *ai.dim.relative_to );
                std::shared_ptr< image_view_t > view;
                if( image_or_tex.index() == 0 ) {
                  view = resource->image->get( std::get< image_pool::image_descriptor >( image_or_tex ) );
                }
                else {
                  view = resource->texture->get( std::get< texture_pool::texture_descriptor >( image_or_tex ) ).first;
                }
                const auto s = view->get_factory()->get_props().get_basic().extent;
                size.x = s.width;
                size.y = s.height;
                size.z = s.depth;
                if( view->get_factory()->get_props().get_basic().imageType == vk::ImageType::e1D ) {
                  size.y = view->get_props().get_basic().subresourceRange.layerCount;
                  size.z = 0.0f;
                  image_type = vk::ImageType::e1D;
                  layer_count = view->get_props().get_basic().subresourceRange.layerCount;
                }
                if( view->get_factory()->get_props().get_basic().imageType == vk::ImageType::e2D ) {
                  size.z = view->get_props().get_basic().subresourceRange.layerCount;
                  image_type = vk::ImageType::e2D;
                  layer_count = view->get_props().get_basic().subresourceRange.layerCount;
                }
                else {
                  image_type = vk::ImageType::e3D;
                }
                format = view->get_factory()->get_props().get_basic().format;
              }
              else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
                throw exception::runtime_error( "shader_graph_builder::assign_image : image size depends on unknown image "+*ai.dim.relative_to, __FILE__, __LINE__ );
              }
              else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
                const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
                if( *ai.dim.relative_to == create_info->input_name ) {
                  const auto view = resource->image->get( create_info->input );
                  const auto s = view->get_factory()->get_props().get_basic().extent;
                  size.x = s.width;
                  size.y = s.height;
                  size.z = s.depth;
                  if( view->get_factory()->get_props().get_basic().imageType == vk::ImageType::e1D ) {
                    size.y = view->get_props().get_basic().subresourceRange.layerCount;
                    size.z = 0.0f;
                    image_type = vk::ImageType::e1D;
                    layer_count = view->get_props().get_basic().subresourceRange.layerCount;
                  }
                  if( view->get_factory()->get_props().get_basic().imageType == vk::ImageType::e2D ) {
                    size.z = view->get_props().get_basic().subresourceRange.layerCount;
                    image_type = vk::ImageType::e2D;
                    layer_count = view->get_props().get_basic().subresourceRange.layerCount;
                  }
                  else {
                    image_type = vk::ImageType::e3D;
                  }
                }
                else {
                  throw exception::runtime_error( "shader_graph_builder::assign_image : image size depends on unknown image "+*ai.dim.relative_to, __FILE__, __LINE__ );
                }
              }
            }
            auto sized_basic = ai.allocate_info.create_info.get_basic();
            if( image_type ) {
              sized_basic.setImageType( *image_type );
            }
            
            auto modified = ai.allocate_info;
            size = ai.dim.size_transform * size;
            size /= size.w;
            auto layer_count_vec = ( glm::vec2( layer_count, 1.0f ) * ai.dim.layer_transform );
            layer_count = std::max( layer_count_vec.x / layer_count_vec.y, 1.0f );
            if( ai.dim.preserve_layer_count && sized_basic.imageType == vk::ImageType::e2D ) {
              size.z = layer_count;
            }
            else if( ai.dim.preserve_layer_count && sized_basic.imageType == vk::ImageType::e1D ) {
              size.y = layer_count;
            }
            
            if( sized_basic.format == vk::Format::eUndefined && format ) {
              sized_basic.format = *format;
            }
            if( sized_basic.imageType == vk::ImageType::e1D ) {
              sized_basic.extent.width = std::max( size.x, 1.0f );
              sized_basic.extent.height = 1;
              sized_basic.extent.depth = 1;
              sized_basic.arrayLayers = std::max( size.y, 1.0f );
              sized_basic.mipLevels = 1;
            }
            else if( sized_basic.imageType == vk::ImageType::e2D ) {
              sized_basic.extent.width = std::max( size.x, 1.0f );
              sized_basic.extent.height = std::max( size.y, 1.0f );
              sized_basic.extent.depth = 1;
              sized_basic.arrayLayers = std::max( size.z, 1.0f );
              sized_basic.mipLevels = 1;
            }
            else if( sized_basic.imageType == vk::ImageType::e2D ) {
              sized_basic.extent.width = std::max( size.x, 1.0f );
              sized_basic.extent.height = std::max( size.y, 1.0f );
              sized_basic.extent.depth = std::max( size.z, 1.0f );
              sized_basic.arrayLayers = 1;
              sized_basic.mipLevels = 1;
            }
            modified.create_info.set_basic( sized_basic );
            bool solved = false;
            for( auto cur = binding.begin(); cur != binding.end(); ++cur ) {
              cur = std::find_if(
                cur,
                binding.end(),
                [&]( const auto &b ) -> bool {
                  return *b.allocate_info == modified;
                }
              );
              if( cur == binding.end() ) {
                break;
              }
              else {
                if( shareable( *cur->used_by, v ) ) {
                  reuse( cur, v, name, texture );
                  cur->shareable = create_info->is_shareable( name );
                  solved = true;
                  break;
                }
              }
            }
            if( !solved ) {
              const auto view = resource->image->allocate( modified ).linear;
              const bool shareable = create_info->is_shareable( name );
              bind( v, name, view, modified, shareable, texture );
            }
          }
          else if( out.second.index() == 0u ) {
            const auto &desc = std::get< image_pool::image_descriptor >( out.second );
            const auto view = resource->image->get( desc );
            bind( v, name, desc,
              image_allocate_info()
                .set_create_info( view->get_factory()->get_props() )
                .set_range(
                  subview_range()
                    .set_mip_offset( view->get_props().get_basic().subresourceRange.baseMipLevel )
                    .set_mip_count( view->get_props().get_basic().subresourceRange.levelCount )
                    .set_layer_offset( view->get_props().get_basic().subresourceRange.baseArrayLayer )
                    .set_layer_count( view->get_props().get_basic().subresourceRange.layerCount )
                ),
              false,
              texture
            );
          }
        }
      }
      else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
        const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ v ].command );
        const std::string &name = create_info->name;
        auto &out = create_info->output;
        if( out.index() == 1u ) {
          const auto &ai = std::get< image_allocate_info >( out );
          bool solved = false;
          for( auto cur = binding.begin(); cur != binding.end(); ++cur ) {
            cur = std::find_if(
              cur,
              binding.end(),
              [&]( const auto &b ) -> bool {
                return *b.allocate_info == ai;
              }
            );
            if( cur == binding.end() ) {
              break;
            }
            else {
              if( shareable( *cur->used_by, v ) ) {
                reuse( cur, v, name, texture );
                cur->shareable = create_info->shareable;
                solved = true;
                break;
              }
            }
          }
          if( !solved ) {
            const auto view = resource->image->allocate( ai ).linear;
            const bool shareable = create_info->shareable;
            bind( v, name, view, ai, shareable, texture );
          }
        }
        else if( out.index() == 0u ) {
          const auto &desc = std::get< image_pool::image_descriptor >( out );
          const auto view = resource->image->get( desc );
          const bool shareable = create_info->shareable;
          bind( v, name, desc,
            image_allocate_info()
              .set_create_info( view->get_factory()->get_props() )
              .set_range(
                subview_range()
                  .set_mip_offset( view->get_props().get_basic().subresourceRange.baseMipLevel )
                  .set_mip_count( view->get_props().get_basic().subresourceRange.levelCount )
                  .set_layer_offset( view->get_props().get_basic().subresourceRange.baseArrayLayer )
                  .set_layer_count( view->get_props().get_basic().subresourceRange.layerCount )
              ),
            shareable,
            texture
          );
        }
      }
      else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
        const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
        const std::string &name = create_info->output_name;
        auto &out = create_info->output;
        if( out.index() == 1u ) {
          const auto &ai = std::get< image_allocate_info >( out );
          bool solved = false;
          for( auto cur = binding.begin(); cur != binding.end(); ++cur ) {
            cur = std::find_if(
              cur,
              binding.end(),
              [&]( const auto &b ) -> bool {
                return *b.allocate_info == ai;
              }
            );
            if( cur == binding.end() ) {
              break;
            }
            else {
              if( shareable( *cur->used_by, v ) ) {
                reuse( cur, v, name, texture );
                cur->shareable = create_info->shareable;
                solved = true;
                break;
              }
            }
          }
          if( !solved ) {
            const auto view = resource->image->allocate( ai ).linear;
            const bool shareable = create_info->shareable;
            bind( v, name, view, ai, shareable, texture );
          }
        }
        else if( out.index() == 0u ) {
          const auto &desc = std::get< image_pool::image_descriptor >( out );
          const auto view = resource->image->get( desc );
          const bool shareable = create_info->shareable;
          bind( v, name, desc,
            image_allocate_info()
              .set_create_info( view->get_factory()->get_props() )
              .set_range(
                subview_range()
                  .set_mip_offset( view->get_props().get_basic().subresourceRange.baseMipLevel )
                  .set_mip_count( view->get_props().get_basic().subresourceRange.levelCount )
                  .set_layer_offset( view->get_props().get_basic().subresourceRange.baseArrayLayer )
                  .set_layer_count( view->get_props().get_basic().subresourceRange.layerCount )
              ),
            shareable,
            texture
          );
        }
      }
      const auto [out_begin,out_end] = out_edges( v, *graph );
      for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
        const auto next = target( edge, *graph );
        if( visited.find( next ) == visited.end() ) {
          v_cur.push( next );
          visited.insert( next );
        }
      }
      v_cur.pop();
    }
  }
  std::pair< bool, unsigned int > shader_graph_builder::is_ready_to_execute(
    const std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v,
    const image_to_texture_map &texture
  ) {
    unsigned int score = 0;
    if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command );
      for( const auto &i: create_info->get_input() ) {
        const auto &is = state.find( i.second );
        if( is != state.end() ) {
          const bool expected = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          ) != is->second.expected_consumer.end();
          if( !expected ) {
            return std::make_pair( false, 0 );
          }
          const bool ready =
            is->second.mode == image_mode::writable_without_sync ||
            is->second.mode == image_mode::readable_after_sync ||
            is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
            is->second.mode == image_mode::readable_after_sync_writable_after_sync;
          if( !ready ) {
            return std::make_pair( false, 0 );
          }
          if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
            ++score;
          }
        }
      }
      for( const auto &i: create_info->get_sampled() ) {
        const auto i2t = std::find_if(
          texture.begin(),
          texture.end(),
          [&]( const auto &t ) { return t.second == i.second; }
        );
        if( i2t == texture.end() ) {
          throw exception::runtime_error( "shader_graph_builder::is_ready_to_execute : Sampled texture " + std::to_string( *i.second ) + " is not known", __FILE__, __LINE__ );
        }
        const auto &is = state.find( i2t->first.first );
        if( is != state.end() ) {
          const bool expected = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          ) != is->second.expected_consumer.end();
          if( !expected ) {
            return std::make_pair( false, 0 );
          }
          const bool ready =
            is->second.mode == image_mode::writable_without_sync ||
            is->second.mode == image_mode::readable_after_sync ||
            is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
            is->second.mode == image_mode::readable_after_sync_writable_after_sync;
          if( !ready ) {
            return std::make_pair( false, 0 );
          }
          if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
            ++score;
          }
        }
      }
      for( const auto &i: create_info->get_inout() ) {
        const auto &is = state.find( i.second );
        if( is != state.end() ) {
          const bool expected = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          ) != is->second.expected_consumer.end();
          if( !expected ) {
            return std::make_pair( false, 0 );
          }
          const bool ready =
            is->second.mode == image_mode::writable_without_sync ||
            is->second.mode == image_mode::readable_after_sync ||
            is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
            is->second.mode == image_mode::readable_without_sync_writable_after_sync;
          if( !ready ) {
            return std::make_pair( false, 0 );
          }
          if( is->second.mode != image_mode::readable_after_sync_writable_after_sync ) {
            score += 256;
          }
        }
      }
      for( const auto &o: create_info->get_plan().output ) {
        const auto &is = state.find( std::get< image_pool::image_descriptor >( o.second ) );
        if( is != state.end() ) {
          if( is->second.generators->size() <= is->second.next_generator ) {
            throw exception::runtime_error( "shader_graph_builder::is_ready_to_execute : No more generators are expected on this image", __FILE__, __LINE__ );
          }
          const auto &expected_generator = (*is->second.generators)[ is->second.next_generator ];
          if( expected_generator.first != v ) {
            return std::make_pair( false, 0 );
          }
          const bool ready =
            is->second.mode == image_mode::writable_without_sync ||
            is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
            is->second.mode == image_mode::readable_after_sync_writable_after_sync;
          if( !ready ) {
            return std::make_pair( false, 0 );
          }
          if( is->second.mode != image_mode::writable_without_sync ) {
            score += 65536;
          }
        }
      }
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ v ].command );
      const auto &o = create_info->output;
      const auto &is = state.find( std::get< image_pool::image_descriptor >( o ) );
      if( is != state.end() ) {
        if( is->second.generators->size() <= is->second.next_generator ) {
          throw exception::runtime_error( "shader_graph_builder::is_ready_to_execute : No more generators are expected on this image", __FILE__, __LINE__ );
        }
        const auto &expected_generator = (*is->second.generators)[ is->second.next_generator ];
        if( expected_generator.first != v ) {
          return std::make_pair( false, 0 );
        }
        const bool ready =
          is->second.mode == image_mode::writable_without_sync ||
          is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
          is->second.mode == image_mode::readable_after_sync_writable_after_sync;
        if( !ready ) {
          return std::make_pair( false, 0 );
        }
        if( is->second.mode != image_mode::writable_without_sync ) {
          score += 65536;
        }
      }
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
      const auto &o = create_info->output;
      const auto &is = state.find( std::get< image_pool::image_descriptor >( o ) );
      if( is != state.end() ) {
        if( is->second.generators->size() <= is->second.next_generator ) {
          throw exception::runtime_error( "shader_graph_builder::is_ready_to_execute : No more generators are expected on this image", __FILE__, __LINE__ );
        }
        const auto &expected_generator = (*is->second.generators)[ is->second.next_generator ];
        if( expected_generator.first != v ) {
          return std::make_pair( false, 0 );
        }
        const bool ready =
          is->second.mode == image_mode::writable_without_sync ||
          is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
          is->second.mode == image_mode::readable_after_sync_writable_after_sync;
        if( !ready ) {
          return std::make_pair( false, 0 );
        }
        if( is->second.mode != image_mode::writable_without_sync ) {
          score += 65536;
        }
      }
    }
    return std::make_pair( true, score );
  }
  shader_graph_command_list shader_graph_builder::run(
    std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v,
    const image_to_texture_map &texture
  ) {
    shader_graph_command_list list;
    if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( (*graph)[ v ].command );
      for( const auto &i: create_info->get_input() ) {
        const auto &is = state.find( i.second );
        if( is != state.end() ) {
          if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
            if( is->second.last_generator_type == image_generator_type::transfer ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask( vk::AccessFlagBits::eTransferWrite )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eTransfer )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::output ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask( vk::AccessFlagBits::eShaderWrite )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::inout ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            is->second.mode = image_mode::readable_without_sync_writable_after_sync;
          }
          const auto consumed = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          );
          if( consumed != is->second.expected_consumer.end() ) {
            is->second.expected_consumer.erase( consumed );
          }
        }
      }
      for( const auto &i: create_info->get_sampled() ) {
        const auto i2t = std::find_if(
          texture.begin(),
          texture.end(),
          [&]( const auto &t ) { return t.second == i.second; }
        );
        const auto &is = state.find( i2t->first.first );
        if( is != state.end() ) {
          if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
            if( is->second.last_generator_type == image_generator_type::transfer ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask( vk::AccessFlagBits::eTransferWrite )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eTransfer )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::output ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask( vk::AccessFlagBits::eShaderWrite )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::inout ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            is->second.mode = image_mode::readable_without_sync_writable_after_sync;
          }
          const auto consumed = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          );
          if( consumed != is->second.expected_consumer.end() ) {
            is->second.expected_consumer.erase( consumed );
          }
        }
      }
      for( const auto &i: create_info->get_inout() ) {
        const auto &is = state.find( i.second );
        if( is != state.end() ) {
          if( is->second.mode != image_mode::readable_after_sync_writable_after_sync ) {
            if( is->second.last_generator_type == image_generator_type::transfer ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask(
                            vk::AccessFlagBits::eTransferRead |
                            vk::AccessFlagBits::eTransferWrite
                          )
                          .set_dest_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_src_stage( vk::PipelineStageFlagBits::eTransfer )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::output ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_dest_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::inout ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_dest_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            is->second.mode = image_mode::readable_after_sync_writable_after_sync;
          }
          const auto consumed = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          );
          if( consumed != is->second.expected_consumer.end() ) {
            is->second.expected_consumer.erase( consumed );
          }
          is->second.last_generator_type = image_generator_type::inout;
          const auto [out_begin,out_end] = out_edges( v, *graph );
          for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
            const auto &sub = (*graph)[ edge ];
            const auto match = std::find_if(
              sub.begin(),
              sub.end(),
              [&i]( const auto &e ) {
                return e.from == i.first;
              }
            );
            if( match != sub.end() ) {
              const auto dest = target( edge, *graph );
              const auto existing = std::find(
                is->second.next_expected_consumer.begin(),
                is->second.next_expected_consumer.end(),
                dest
              );
              if( existing == is->second.next_expected_consumer.end() ) {
                is->second.next_expected_consumer.push_back( dest );
              }
              if( is->second.expected_consumer.empty() ) {
                std::swap(
                  is->second.expected_consumer,
                  is->second.next_expected_consumer
                );
              }
            }
          }
        }
      }
      for( const auto &o: create_info->get_plan().output ) {
        const auto &is = state.find( std::get< image_pool::image_descriptor >( o.second ) );
        if( is != state.end() ) {
          if( is->second.mode != image_mode::writable_without_sync ) {
            list.push_back(
              shader_graph_barrier()
                .set_state(
                  barrier_state()
                    .set_config(
                      barrier_config()
                        .set_src_access_mask(
                          vk::AccessFlagBits::eTransferRead |
                          vk::AccessFlagBits::eTransferWrite |
                          vk::AccessFlagBits::eShaderRead |
                          vk::AccessFlagBits::eShaderWrite
                        )
                        .set_dest_access_mask( vk::AccessFlagBits::eShaderWrite )
                        .set_src_stage(
                          vk::PipelineStageFlagBits::eTransfer |
                          vk::PipelineStageFlagBits::eComputeShader
                        )
                        .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                    )
                    .set_data(
                      resource->image->get( is->first )
                    )
                )
                .add_view( is->first )
            );
            is->second.mode = image_mode::writable_without_sync;
          }
          ++is->second.next_generator;
          is->second.last_generator_type = image_generator_type::output;
          const auto [out_begin,out_end] = out_edges( v, *graph );
          for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
            const auto &sub = (*graph)[ edge ];
            const auto match = std::find_if(
              sub.begin(),
              sub.end(),
              [&o]( const auto &e ) {
                return e.from == o.first;
              }
            );
            if( match != sub.end() ) {
              const auto dest = target( edge, *graph );
              const auto existing = std::find(
                is->second.next_expected_consumer.begin(),
                is->second.next_expected_consumer.end(),
                dest
              );
              if( existing == is->second.next_expected_consumer.end() ) {
                is->second.expected_consumer.push_back( dest );
              }
              //break;
            }
          }
        }
      }
      list.push_back(
        std::make_shared< image_io >(
          *create_info
        )
      );
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( (*graph)[ v ].command );
      const auto &o = create_info->output;
      const auto &is = state.find( std::get< image_pool::image_descriptor >( o ) );
      if( is != state.end() ) {
        if( is->second.mode != image_mode::writable_without_sync ) {
          list.push_back(
            shader_graph_barrier()
              .set_state(
                barrier_state()
                  .set_config(
                    barrier_config()
                      .set_src_access_mask(
                        vk::AccessFlagBits::eTransferRead |
                        vk::AccessFlagBits::eTransferWrite |
                        vk::AccessFlagBits::eShaderRead |
                        vk::AccessFlagBits::eShaderWrite
                      )
                      .set_dest_access_mask( vk::AccessFlagBits::eTransferWrite )
                      .set_src_stage(
                        vk::PipelineStageFlagBits::eTransfer |
                        vk::PipelineStageFlagBits::eComputeShader
                      )
                      .set_dest_stage( vk::PipelineStageFlagBits::eTransfer )
                  )
                  .set_data(
                    resource->image->get( is->first )
                  )
              )
              .add_view( is->first )
          );
          is->second.mode = image_mode::writable_without_sync;
        }
        ++is->second.next_generator;
        is->second.last_generator_type = image_generator_type::transfer;
        const auto [out_begin,out_end] = out_edges( v, *graph );
        for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
          const auto &sub = (*graph)[ edge ];
          const auto match = std::find_if(
            sub.begin(),
            sub.end(),
            [&]( const auto &e ) {
              return e.from == create_info->name;
            }
          );
          if( match != sub.end() ) {
            const auto dest = target( edge, *graph );
            const auto existing = std::find(
              is->second.next_expected_consumer.begin(),
              is->second.next_expected_consumer.end(),
              dest
            );
            if( existing == is->second.next_expected_consumer.end() ) {
              is->second.expected_consumer.push_back( dest );
            }
            break;
          }
        }
      }
      list.push_back(
        create_info
      );
    }
    else if( shader_graph_vertex_command_id( (*graph)[ v ].command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( (*graph)[ v ].command );
      {
        const auto &is = state.find( create_info->input );
        if( is != state.end() ) {
          if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
            if( is->second.last_generator_type == image_generator_type::transfer ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask( vk::AccessFlagBits::eTransferWrite )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eTransfer )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::output ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask( vk::AccessFlagBits::eShaderWrite )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            else if( is->second.last_generator_type == image_generator_type::inout ) {
              list.push_back(
                shader_graph_barrier()
                  .set_state(
                    barrier_state()
                      .set_config(
                        barrier_config()
                          .set_src_access_mask(
                            vk::AccessFlagBits::eShaderRead |
                            vk::AccessFlagBits::eShaderWrite
                          )
                          .set_dest_access_mask( vk::AccessFlagBits::eShaderRead )
                          .set_src_stage( vk::PipelineStageFlagBits::eComputeShader )
                          .set_dest_stage( vk::PipelineStageFlagBits::eComputeShader )
                      )
                      .set_data(
                        resource->image->get( is->first )
                      )
                  )
                  .add_view( is->first )
              );
            }
            is->second.mode = image_mode::readable_without_sync_writable_after_sync;
          }
          const auto consumed = std::find(
            is->second.expected_consumer.begin(),
            is->second.expected_consumer.end(),
            v
          );
          if( consumed != is->second.expected_consumer.end() ) {
            is->second.expected_consumer.erase( consumed );
          }
        }
      }
      {
        const auto &o = create_info->output;
        const auto &is = state.find( std::get< image_pool::image_descriptor >( o ) );
        if( is != state.end() ) {
          if( is->second.mode != image_mode::writable_without_sync ) {
            list.push_back(
              shader_graph_barrier()
                .set_state(
                  barrier_state()
                    .set_config(
                      barrier_config()
                        .set_src_access_mask(
                          vk::AccessFlagBits::eTransferRead |
                          vk::AccessFlagBits::eTransferWrite |
                          vk::AccessFlagBits::eShaderRead |
                          vk::AccessFlagBits::eShaderWrite
                        )
                        .set_dest_access_mask( vk::AccessFlagBits::eTransferWrite )
                        .set_src_stage(
                          vk::PipelineStageFlagBits::eTransfer |
                          vk::PipelineStageFlagBits::eComputeShader
                        )
                        .set_dest_stage( vk::PipelineStageFlagBits::eTransfer )
                    )
                    .set_data(
                      resource->image->get( is->first )
                    )
                )
                .add_view( is->first )
            );
            is->second.mode = image_mode::writable_without_sync;
          }
          ++is->second.next_generator;
          is->second.last_generator_type = image_generator_type::transfer;
          const auto [out_begin,out_end] = out_edges( v, *graph );
          for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
            const auto &sub = (*graph)[ edge ];
            const auto match = std::find_if(
              sub.begin(),
              sub.end(),
              [&]( const auto &e ) {
                return e.from == create_info->output_name;
              }
            );
            if( match != sub.end() ) {
              const auto dest = target( edge, *graph );
              const auto existing = std::find(
                is->second.next_expected_consumer.begin(),
                is->second.next_expected_consumer.end(),
                dest
              );
              if( existing == is->second.next_expected_consumer.end() ) {
                is->second.expected_consumer.push_back( dest );
              }
              break;
            }
          }
        }
      }
      list.push_back(
        create_info
      );
    }
    return list;
  }
  shader_graph_command_list shader_graph_builder::build_command_list() {
    image_to_texture_map texture;
    if( binding.empty() ) {
      assign_image( texture );
    }
    shader_graph_command_list cl;
    const auto [v_begin,v_end] = vertices( *graph );
    std::multimap< unsigned int, graph_type::vertex_descriptor > v_cur;
    std::unordered_set< graph_type::vertex_descriptor > v_next;
    std::unordered_set< graph_type::vertex_descriptor > visited;
    std::unordered_map< image_pool::image_descriptor, image_state > is;
    for( const auto &b: binding ) {
      is.insert( std::make_pair( b.view, image_state( b ) ) );
    }
    for( const auto &v: independent_vertex ) {
      const auto [ready,priority] = is_ready_to_execute( is, v, texture );
      if( !ready ) {
        throw exception::runtime_error( "shader_graph_builder::build_command_list : Independent verex should always ready to execute", __FILE__, __LINE__ );
      }
      v_cur.insert( std::make_pair( priority, v ) );
      visited.insert( v );
    }
    if( v_cur.empty() ) {
      throw exception::runtime_error( "shader_graph_builder::build_command_list : At least one vertex must be independent", __FILE__, __LINE__ );
    }
    shader_graph_command_list list;
    while( !v_cur.empty() ) {
      v_next.clear();
      while( !v_cur.empty() ) {
        const auto cur = v_cur.begin();
        const auto [priority,v] = *cur;
        v_cur.erase( cur );
        auto partial_list = run( is, v, texture );
        list.insert( list.end(), partial_list.begin(), partial_list.end() );
        const auto [out_begin,out_end] = out_edges( v, *graph );
        for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
          const auto next = target( edge, *graph );
          if( visited.find( next ) == visited.end() ) {
            v_next.insert( next );
          }
        }
      }
      for( const auto &next: v_next ) {
        const auto [ready,priority] = is_ready_to_execute( is, next, texture );
        if( ready ) {
          if( visited.find( next ) == visited.end() ) {
            v_cur.insert( std::make_pair( priority, next ) );
            visited.insert( next );
          }
        }
      }
    }
    return list;
  }
  void shader_graph_builder::output( const shader_graph_vertex::subresult_type &r ) {
    const auto &v = (*graph)[ r.vertex_id ];
    if( shader_graph_vertex_command_id( v.command.index() ) == shader_graph_vertex_command_id::call ) {
      auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( v.command );
      create_info->set_shareable( r.name, false );
    }
    else if( shader_graph_vertex_command_id( v.command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( v.command );
      create_info->set_shareable( false );
    }
    else if( shader_graph_vertex_command_id( v.command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( v.command );
      create_info->set_shareable( false );
    }
    else {
      throw exception::invalid_argument( "compiled_shader_graph::get_view : broken vertex", __FILE__, __LINE__ );
    }
  }
    
  void compiled_shader_graph::operator()(
    command_buffer_recorder_t &rec
  ) const {
    for( const auto &c: list ) {
      if( shader_graph_command_id( c.index() ) == shader_graph_command_id::call ) {
        (*std::get< std::shared_ptr< image_io > >( c ))( rec );
      }
      else if( shader_graph_command_id( c.index() ) == shader_graph_command_id::barrier ) {
        rec.barrier( std::get< shader_graph_barrier >( c ).state );
      }
      else if( shader_graph_command_id( c.index() ) == shader_graph_command_id::fill ) {
        const auto fill = std::get< std::shared_ptr< image_fill_create_info > >( c );
        rec.fill( resource->image->get( std::get< image_pool::image_descriptor >( fill->output ) ), fill->color );
      }
      else if( shader_graph_command_id( c.index() ) == shader_graph_command_id::blit ) {
        const auto blit = std::get< std::shared_ptr< image_blit_create_info > >( c );
        rec.blit(
          resource->image->get( blit->input ),
          resource->image->get( std::get< image_pool::image_descriptor >( blit->output ) ),
          blit->region,
          blit->filter
        );
      }
    }
  }
  std::shared_ptr< image_view_t > compiled_shader_graph::get_view( const shader_graph_vertex::subresult_type &r ) const {
    const auto &v = (*graph)[ r.vertex_id ];
    if( shader_graph_vertex_command_id( v.command.index() ) == shader_graph_vertex_command_id::call ) {
      const auto &create_info = std::get< std::shared_ptr< image_io_create_info > >( v.command );
      const auto image_or_tex = create_info->get( r.name );
      if( image_or_tex.index() == 0u ) {
        const auto desc = std::get< image_pool::image_descriptor >( image_or_tex );
        if( !desc ) {
          throw exception::invalid_argument( "compiled_shader_graph::get_view : unknown image " + r.name, __FILE__, __LINE__ );
        }
        return resource->image->get( desc );
      }
      else if( image_or_tex.index() == 1u ) {
        const auto desc = std::get< texture_pool::texture_descriptor >( image_or_tex );
        if( !desc ) {
          throw exception::invalid_argument( "compiled_shader_graph::get_view : unknown image " + r.name, __FILE__, __LINE__ );
        }
        return resource->texture->get( desc ).first;
      }
    }
    else if( shader_graph_vertex_command_id( v.command.index() ) == shader_graph_vertex_command_id::fill ) {
      const auto &create_info = std::get< std::shared_ptr< image_fill_create_info > >( v.command );
      if( create_info->name != r.name ) {
          throw exception::invalid_argument( "compiled_shader_graph::get_view : unknown image " + r.name, __FILE__, __LINE__ );
      }
      if( create_info->output.index() != 0 ) {
          throw exception::invalid_argument( "compiled_shader_graph::get_view : image " + r.name + " is not allocated", __FILE__, __LINE__ );
      }
      return resource->image->get( std::get< image_pool::image_descriptor >( create_info->output ) );
    }
    else if( shader_graph_vertex_command_id( v.command.index() ) == shader_graph_vertex_command_id::blit ) {
      const auto &create_info = std::get< std::shared_ptr< image_blit_create_info > >( v.command );
      if( create_info->output_name != r.name ) {
          throw exception::invalid_argument( "compiled_shader_graph::get_view : unknown image " + r.name, __FILE__, __LINE__ );
      }
      if( create_info->output.index() != 0 ) {
          throw exception::invalid_argument( "compiled_shader_graph::get_view : image " + r.name + " is not allocated", __FILE__, __LINE__ );
      }
      return resource->image->get( std::get< image_pool::image_descriptor >( create_info->output ) );
    }
    throw exception::invalid_argument( "compiled_shader_graph::get_view : broken vertex", __FILE__, __LINE__ );
  }
}

