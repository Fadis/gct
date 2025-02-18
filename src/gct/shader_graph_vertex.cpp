#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/properties.hpp>
#include <boost/range/iterator_range.hpp>
#include <gct/exception.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/shader_graph_vertex.hpp>

namespace gct {
  shader_graph_vertex::subresult_type shader_graph_vertex::result_type::operator[]( const std::string name ) const {
    if( create_info->get_plan().output.find( name ) == create_info->get_plan().output.end() ) {
      throw exception::invalid_argument( "shader_graph_vertex::result_type::operator[] : Output image " + name + "does not exist", __FILE__, __LINE__ );
    }
    return shader_graph_vertex::subresult_type(
      vertex_id,
      create_info,
      name
    );
  }
  shader_graph_vertex::result_type shader_graph_vertex::operator()(
    const std::unordered_map< std::string, subresult_type > &input
  ) {
    if( called ) {
      throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Vertex must not be called multiple times", __FILE__, __LINE__ );
    }
    for( const auto &i: input ) {
      if( create_info->get_plan().input.find( i.first ) == create_info->get_plan().input.end() ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Input image " + i.first + "does not exist", __FILE__, __LINE__ );
      }
    }
    std::unordered_map< graph_type::vertex_descriptor, graph_type::edge_descriptor > unique_edge;
    for( const auto &i: input ) {
      if( unique_edge.find( i.second.vertex_id ) == unique_edge.end() ) {
        auto [desc,inserted] = add_edge( i.second.vertex_id, vertex_id, *graph );
        if( !inserted ) {
          throw exception::runtime_error( "shader_graph_vertex::result_type::operator() : Edge insertion from " + i.second.name + " to " + i.first + " failed.", __FILE__, __LINE__ );
        }
        unique_edge.insert( std::make_pair( i.second.vertex_id, desc ) );
      }
    }
    for( const auto &i: input ) {
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
      create_info
    );
  }
  std::vector< std::pair< shader_graph_optimizer::graph_type::vertex_descriptor, std::string > >
  shader_graph_optimizer::get_consumer_of(
    const graph_type::vertex_descriptor &generator,
    const std::string &name
  ) const {
    std::vector< std::pair< shader_graph_optimizer::graph_type::vertex_descriptor, std::string > > temp;
    const auto [out_begin,out_end] = out_edges( generator, *graph );
    for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
      for( const auto &sub: (*graph)[ edge ] ) {
        if( sub.from == name ) {
          temp.emplace_back( target( edge, *graph ), sub.to );
        }
      }
    }
    return temp;
  }
  bool shader_graph_optimizer::shareable(
    const graph_type::vertex_descriptor &first_generator,
    const std::string &first_subedge,
    const graph_type::vertex_descriptor &second_generator
  ) const {
    for( const auto &v: get_consumer_of( first_generator, first_subedge ) ) {
      if( !boost::is_reachable( v.first, second_generator, *graph, boost::get( &shader_graph_vertex_type::color, *graph ) ) ) {
        return false;
      }
    }
    return true;
  }
  bool shader_graph_optimizer::shareable(
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
  void shader_graph_optimizer::assign_image() {
    const auto [v_begin,v_end] = vertices( *graph );
    for( const auto &v: boost::make_iterator_range( v_begin, v_end ) ) {
      for( const auto &out: (*graph)[ v ].create_info->get_plan().output ) {
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
              if( shareable( cur->used_by, v ) ) {
                // share
                cur->used_by.emplace_back( v, name );
                solved = true;
                break;
              }
            }
          }
          if( !solved ) {
            // add view
          }
        }
      }
    }
  }
}

