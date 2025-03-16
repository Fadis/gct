#include <queue>
#include <nlohmann/json.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/properties.hpp>
#include <boost/range/iterator_range.hpp>
#include <gct/exception.hpp>
#include <gct/compute.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/image_io.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/shader_graph.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const image_mode &src ) {
  if( src == image_mode::writable_without_sync ) {
    dest = "writable_without_sync";
  }
  else if( src == image_mode::readable_after_sync ) {
    dest = "readable_after_sync";
  }
  else if( src == image_mode::readable_without_sync_writable_after_sync ) {
    dest = "readable_without_sync_writable_after_sync";
  }
  else if( src == image_mode::reaadble_after_sync_writable_after_sync ) {
    dest = "reaadble_after_sync_writable_after_sync";
  }
  else {
    dest = "unknown";
  }
}

void to_json( nlohmann::json &dest, const shader_graph_read_to_write_barrier &src ) {
  dest = nlohmann::json::array();
  for( const auto &v: src.view ) {
    dest.push_back( *v );
  }
}

void to_json( nlohmann::json &dest, const shader_graph_write_to_read_barrier &src ) {
  dest = nlohmann::json::array();
  for( const auto &v: src.view ) {
    dest.push_back( *v );
  }
}

void to_json( nlohmann::json &dest, const shader_graph_command &src ) {
  if( src.index() == 0 ) {
    dest = *std::get< std::shared_ptr< image_io > >( src );
  }
  else if( src.index() == 1 ) {
    dest = std::get< shader_graph_read_to_write_barrier >( src );
  }
  else if( src.index() == 2 ) {
    dest = std::get< shader_graph_write_to_read_barrier >( src );
  }
}

void to_json( nlohmann::json &dest, const shader_graph_command_list &src ) {
  dest = nlohmann::json::array();
  for( const auto &v: src ) {
    dest.push_back( v );
  }
}

void to_json( nlohmann::json &dest, const compiled_shader_graph &src ) {
  dest = src.get_list();
}

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
  std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > >
  shader_graph_builder::get_consumer_of(
    const graph_type::vertex_descriptor &generator,
    const std::string &name
  ) const {
    std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > > temp;
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
  bool shader_graph_builder::shareable(
    const graph_type::vertex_descriptor &first_generator,
    const std::string &first_subedge,
    const graph_type::vertex_descriptor &second_generator
  ) const {
    for( const auto &v: get_consumer_of( first_generator, first_subedge ) ) {
      std::cout << first_generator << ":" << first_subedge << " " << v.first << ":" << v.second << " " << second_generator << std::endl;
      if( v.first == second_generator ) {
        std::cout << "  failed1" << std::endl;
        return false;
      }
      if( !boost::is_reachable( v.first, second_generator, *graph, boost::get( &shader_graph_vertex_type::color, *graph ) ) ) {
        std::cout << "  failed2" << std::endl;
        return false;
      }
    }
    std::cout << "  ok" << std::endl;
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
    const std::string &name
  ) {
    iter->add_used_by( v, name );
    (*graph)[ v ].create_info->add_output( name, iter->view );
    const auto [e_begin,e_end] = out_edges( v, *graph );
    for( const auto &edge: boost::make_iterator_range( e_begin, e_end ) ) {
      for( const auto &sub: (*graph)[ edge ] ) {
        if( sub.from == name ) {
          const auto consumer = target( edge, *graph );
          (*graph)[ consumer ].create_info->add_input( sub.to, iter->view );
          break;
        }
      }
    }
  }
  void shader_graph_builder::bind(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    const image_allocate_info &ai
  ) {
    binding.push_back(
      image_binding()
        .set_allocate_info( std::make_shared< image_allocate_info >( ai ) )
        .set_view( view )
        .add_used_by( v, name ) 
    );
    (*graph)[ v ].create_info->add_output( name, view );
    const auto [e_begin,e_end] = out_edges( v, *graph );
    for( const auto &edge: boost::make_iterator_range( e_begin, e_end ) ) {
      for( const auto &sub: (*graph)[ edge ] ) {
        if( sub.from == name ) {
          const auto consumer = target( edge, *graph );
          (*graph)[ consumer ].create_info->add_input( sub.to, view );
          break;
        }
      }
    }
  }
  void shader_graph_builder::assign_image() {
    const auto [v_begin,v_end] = vertices( *graph );
    std::queue< graph_type::vertex_descriptor > v_cur;
    std::unordered_set< graph_type::vertex_descriptor > visited;
    for( const auto &v: boost::make_iterator_range( v_begin, v_end ) ) {
      if( (*graph)[ v ].create_info->independent() ) {
        v_cur.push( v );
        independent_vertex.push_back( v );
        visited.insert( v );
      }
    }
    if( v_cur.empty() ) {
      throw -1;
    }
    while( !v_cur.empty() ) {
      const auto &v = v_cur.front();
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
              if( shareable( *cur->used_by, v ) ) {
                reuse( cur, v, name );
                solved = true;
                break;
              }
            }
          }
          if( !solved ) {
            const auto view = resource->image->allocate( ai ).linear;
            bind( v, name, view, ai );
          }
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
  std::pair< bool, unsigned int > shader_graph_builder::is_ready_to_execulte(
    const std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v
  ) {
    unsigned int score = 0;
    for( const auto &i: (*graph)[ v ].create_info->get_input() ) {
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
        if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
          ++score;
        }
      }
    }
    for( const auto &o: (*graph)[ v ].create_info->get_plan().output ) {
      const auto &is = state.find( std::get< image_pool::image_descriptor >( o.second ) );
      if( is != state.end() ) {
        if( is->second.generators->size() <= is->second.next_generator ) {
          throw -1;
        }
        const auto &expected_generator = (*is->second.generators)[ is->second.next_generator ];
        if( expected_generator.first != v ) {
          return std::make_pair( false, 0 );
        }
        const bool ready =
          is->second.mode == image_mode::writable_without_sync ||
          is->second.mode == image_mode::readable_without_sync_writable_after_sync ||
          is->second.mode == image_mode::reaadble_after_sync_writable_after_sync;
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
    const graph_type::vertex_descriptor &v
  ) {
    shader_graph_command_list list;
    for( const auto &i: (*graph)[ v ].create_info->get_input() ) {
      const auto &is = state.find( i.second );
      if( is != state.end() ) {
        if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
          list.push_back(
            shader_graph_write_to_read_barrier()
              .add_view( is->first )
          );
          is->second.mode = image_mode::readable_without_sync_writable_after_sync;
        }
      }
    }
    for( const auto &o: (*graph)[ v ].create_info->get_plan().output ) {
      const auto &is = state.find( std::get< image_pool::image_descriptor >( o.second ) );
      if( is != state.end() ) {
        if( is->second.mode != image_mode::writable_without_sync ) {
          list.push_back(
            shader_graph_read_to_write_barrier()
              .add_view( is->first )
          );
          is->second.mode = image_mode::writable_without_sync;
        }
        ++is->second.next_generator;
        const auto [out_begin,out_end] = out_edges( v, *graph );
        for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
          for( const auto &e: (*graph)[ edge ] ) {
            if( e.from == o.first ) {
              const auto dest = target( edge, *graph );
              is->second.expected_consumer.push_back( dest );
              break;
            }
          }
        }
      }
    }
    list.push_back(
      std::make_shared< image_io >(
        *(*graph)[ v ].create_info
      )
    );
    return list;
  }
  shader_graph_command_list shader_graph_builder::build_command_list() {
    if( binding.empty() ) {
      assign_image();
    }
    shader_graph_command_list cl;
    const auto [v_begin,v_end] = vertices( *graph );
    std::multimap< unsigned int, graph_type::vertex_descriptor > v_cur;
    std::unordered_set< graph_type::vertex_descriptor > visited;
    std::unordered_map< image_pool::image_descriptor, image_state > is;
    for( const auto &b: binding ) {
      is.insert( std::make_pair( b.view, image_state( b ) ) );
    }
    for( const auto &v: independent_vertex ) {
      const auto [ready,priority] = is_ready_to_execulte( is, v );
      if( !ready ) {
        throw -1;
      }
      v_cur.insert( std::make_pair( priority, v ) );
      visited.insert( v );
    }
    if( v_cur.empty() ) {
      throw -1;
    }
    shader_graph_command_list list;
    while( !v_cur.empty() ) {
      /*std::cout << " v_cur : ";
      for( const auto &v : v_cur ) {
        std::cout << v.first << ":" << nlohmann::json( *(*graph)[v.second].create_info ) << ", ";
      }
      std::cout << std::endl;
      std::cout << "visited : ";
      for( const auto &v: visited ) {
        std::cout << nlohmann::json( *(*graph)[v].create_info ) << ", ";
      }
      std::cout << std::endl;*/
      const auto cur = v_cur.begin();
      const auto &[priority,v] = *cur;
      auto partial_list = run( is, v );
      list.insert( list.end(), partial_list.begin(), partial_list.end() );
      const auto [out_begin,out_end] = out_edges( v, *graph );
      for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
        const auto next = target( edge, *graph );
        if( visited.find( next ) == visited.end() ) {
          const auto [ready,priority] = is_ready_to_execulte( is, next );
          if( ready ) {
            v_cur.insert( std::make_pair( priority, next ) );
            visited.insert( next );
          }
        }
      }
      v_cur.erase( cur );
    }
    return list;
  }
  void compiled_shader_graph::operator()(
    command_buffer_recorder_t &rec
  ) const {
    for( const auto &c: list ) {
      if( c.index() == 0 ) {
        (*std::get< std::shared_ptr< image_io > >( c ))( rec );
      }
      else if( c.index() == 1 ) {
        std::vector< std::shared_ptr< image_view_t > > view;
        view.reserve( std::get< shader_graph_read_to_write_barrier >( c ).view.size() );
        for( const auto &d: std::get< shader_graph_read_to_write_barrier >( c ).view ) {
          view.push_back( resource->image->get( d ) );
        }
        rec.compute_write_barrier(
          syncable()
            .set_image_view( view )
        );
      }
      else if( c.index() == 2 ) {
        std::vector< std::shared_ptr< image_view_t > > view;
        view.reserve( std::get< shader_graph_write_to_read_barrier >( c ).view.size() );
        for( const auto &d: std::get< shader_graph_write_to_read_barrier >( c ).view ) {
          view.push_back( resource->image->get( d ) );
        }
        rec.compute_barrier(
          syncable()
            .set_image_view( view )
        );
      }
    }
  }
}

