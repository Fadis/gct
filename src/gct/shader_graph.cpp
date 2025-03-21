#include <queue>
#include <utility>
#include <nlohmann/json.hpp>
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
  else if( src == image_mode::readable_after_sync_writable_after_sync ) {
    dest = "readable_after_sync_writable_after_sync";
  }
  else {
    dest = "unknown";
  }
}
void to_json( nlohmann::json &dest, const shader_graph_barrier &src ) {
  dest = nlohmann::json::object();
  dest[ "state" ] = src.state;
  dest[ "view" ] = nlohmann::json::array();
  for( const auto &v: src.view ) {
    dest[ "view" ].push_back( *v );
  }
}

void to_json( nlohmann::json &dest, const shader_graph_command &src ) {
  dest = nlohmann::json::object();
  if( src.index() == 0 ) {
    dest[ "type" ] = "image_io";
    dest[ "value" ] = *std::get< std::shared_ptr< image_io > >( src );
  }
  else if( src.index() == 1 ) {
    dest[ "type" ] = "barrier";
    dest[ "value" ] = std::get< shader_graph_barrier >( src );
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
    if( create_info ) {
      if( create_info->get_plan().output.find( name ) == create_info->get_plan().output.end() ) {
        if( create_info->get_plan().inout.find( name ) == create_info->get_plan().inout.end() ) {
          throw exception::invalid_argument( "shader_graph_vertex::result_type::operator[] : Output image " + name + "does not exist", __FILE__, __LINE__ );
        }
      }
      return shader_graph_vertex::subresult_type(
        vertex_id,
        create_info,
        name
      );
    }
    else if( fill_create_info ) {
      if( name != fill_create_info->name ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator[] : Output image " + name + "does not exist", __FILE__, __LINE__ );
      }
      return shader_graph_vertex::subresult_type(
        vertex_id,
        fill_create_info,
        name
      );
    }
    else {
      throw exception::runtime_error( "shader_graph_vertex::result_type::operator[] : Broken result_type", __FILE__, __LINE__ );
    }
  }
  shader_graph_vertex::result_type shader_graph_vertex::operator()(
    const std::unordered_map< std::string, subresult_type > &input
  ) {
    if( called ) {
      throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Vertex must not be called multiple times", __FILE__, __LINE__ );
    }
    std::unordered_set< graph_type::vertex_descriptor > incoming_fill;
    if( create_info ) {
      for( const auto &i: input ) {
        if( create_info->get_plan().input.find( i.first ) == create_info->get_plan().input.end() ) {
          if( create_info->get_plan().inout.find( i.first ) == create_info->get_plan().inout.end() ) {
            throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Input image " + i.first + "does not exist", __FILE__, __LINE__ );
          }
        }
        if( !i.second.create_info && i.second.fill_create_info ) {
          incoming_fill.insert( i.second.vertex_id );
        }
      }
    }
    else if( fill_create_info ) {
      if( !input.empty() ) {
        throw exception::invalid_argument( "shader_graph_vertex::result_type::operator() : Fill does not take any inputs", __FILE__, __LINE__ );
      }
    }
    else {
      throw exception::runtime_error( "shader_graph_vertex::result_type::operator() : Broken result_type", __FILE__, __LINE__ );
    }
    std::unordered_map< graph_type::vertex_descriptor, graph_type::edge_descriptor > unique_edge;
    for( const auto &i: input ) {
      if( unique_edge.find( i.second.vertex_id ) == unique_edge.end() ) {
        auto [desc,inserted] = add_edge( i.second.vertex_id, vertex_id, *graph );
        if( !inserted ) {
          throw exception::runtime_error( "shader_graph_vertex::result_type::operator() : Edge insertion from " + i.second.name + " to " + i.first + " failed.", __FILE__, __LINE__ );
        }
        for( const auto &f : incoming_fill ) {
          auto [desc,inserted] = add_edge( i.second.vertex_id, vertex_id, *graph );
          (*graph)[ f ].fill_create_info->independent = false;
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
    if( create_info ) {
      return result_type(
        graph,
        vertex_id,
        create_info
      );
    }
    else {
      return result_type(
        graph,
        vertex_id,
        fill_create_info
      );
    }
  }
  std::size_t shader_graph_builder::subresult_hash::operator()( const std::pair< graph_type::vertex_descriptor, std::string > &v ) const {
    return std::hash< void* >()( v.first ) ^ std::hash< std::string >()( v.second );
  }
  std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > >
  shader_graph_builder::get_consumer_of(
    const graph_type::vertex_descriptor &generator,
    const std::string &name
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
            if( (*graph)[ dest ].create_info ) {
              const auto inout = std::find_if(
                (*graph)[ dest ].create_info->get_inout().begin(),
                (*graph)[ dest ].create_info->get_inout().end(),
                [&]( const auto &i ) {
                  return i.first == connected->to;
                }
              );
              if( inout != (*graph)[ dest ].create_info->get_inout().end() ) {
                next_depth.insert( std::make_pair( dest, connected->to ) );
              }
              else {
                const auto in = std::find_if(
                  (*graph)[ dest ].create_info->get_input().begin(),
                  (*graph)[ dest ].create_info->get_input().end(),
                  [&]( const auto &i ) {
                    return i.first == connected->to;
                  }
                );
                if( in != (*graph)[ dest ].create_info->get_input().end() ) {
                  consumer.insert( std::make_pair( dest, connected->to ) );
                  consumed.insert( sub );
                }
              }
            }
          }
        }
      }
      for( const auto &sub: consumed ) {
        current_depth.erase( sub );
      }
    } while( !next_depth.empty() );
    consumer.insert( current_depth.begin(), current_depth.end() );
    return std::vector< std::pair< shader_graph_builder::graph_type::vertex_descriptor, std::string > >(
      consumer.begin(),
      consumer.end()
    );
  }
  bool shader_graph_builder::shareable(
    const graph_type::vertex_descriptor &first_generator,
    const std::string &first_subedge,
    const graph_type::vertex_descriptor &second_generator
  ) const {
    const auto &vertex = (*graph)[ first_generator ];
    if( vertex.create_info ) {
      const auto &out = vertex.create_info->get_plan().output.find( first_subedge );
      if( out != vertex.create_info->get_plan().output.end() ) {
        if( out->second.index() == 0 ) {
          const auto view = std::get< image_pool::image_descriptor >( out->second );
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
            throw -1;
          }
        }
        else {
          throw -1;
        }
      }
      else {
        throw -1;
      }
    }
    else if( vertex.fill_create_info ) {
      const auto &out = vertex.fill_create_info->output;
      if( vertex.fill_create_info->name == first_subedge ) {
        if( out.index() == 0 ) {
          const auto view = std::get< image_pool::image_descriptor >( out );
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
            throw -1;
          }
        }
        else {
          throw -1;
        }
      }
      else {
        throw -1;
      }
    }
    else {
      throw -1;
    }
    for( const auto &v: get_consumer_of( first_generator, first_subedge ) ) {
      std::cout << first_generator << ":" << first_subedge << " " << v.first << ":" << v.second << " " << second_generator << std::endl;
      if( v.first == second_generator ) {
        return false;
      }
      if( !boost::is_reachable( v.first, second_generator, *graph, boost::get( &shader_graph_vertex_type::color, *graph ) ) ) {
        return false;
      }
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
    const std::string &name
  ) {
    iter->add_used_by( v, name );
    (*graph)[ v ].create_info->add( name, iter->view );
    const auto [e_begin,e_end] = out_edges( v, *graph );
    for( const auto &edge: boost::make_iterator_range( e_begin, e_end ) ) {
      for( const auto &sub: (*graph)[ edge ] ) {
        if( sub.from == name ) {
          const auto consumer = target( edge, *graph );
          (*graph)[ consumer ].create_info->add( sub.to, iter->view );
          break;
        }
      }
    }
  }
  void shader_graph_builder::bind(
    const graph_type::vertex_descriptor &v,
    const std::string &name,
    const image_pool::image_descriptor &view,
    const image_allocate_info &ai,
    bool shareable
  ) {
    binding.push_back(
      image_binding()
        .set_allocate_info( std::make_shared< image_allocate_info >( ai ) )
        .set_view( view )
        .add_used_by( v, name )
        .set_shareable( shareable )
    );
    if( !(*graph)[ v ].create_info->is_ready( name ) ) {
      (*graph)[ v ].create_info->add( name, view );
    }
    else {
      const auto existing = (*graph)[ v ].create_info->get( name );
      if( !existing ) {
        throw exception::runtime_error( "shader_graph_builder::bind : unable to get bound image view", __FILE__, __LINE__ );
      }
      if( existing != view ) {
        throw exception::runtime_error( "shader_graph_builder::bind : inconsistent image binding", __FILE__, __LINE__ );
      }
    }
    const auto [e_begin,e_end] = out_edges( v, *graph );
    for( const auto &edge: boost::make_iterator_range( e_begin, e_end ) ) {
      for( const auto &sub: (*graph)[ edge ] ) {
        if( sub.from == name ) {
          const auto consumer = target( edge, *graph );
          (*graph)[ consumer ].create_info->add( sub.to, view );
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
      for( const auto &inout: (*graph)[ v ].create_info->get_inout() ) {
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
          false
        );
      }
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
            bind( v, name, view, ai, true );
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
            false
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
  std::pair< bool, unsigned int > shader_graph_builder::is_ready_to_execulte(
    const std::unordered_map< image_pool::image_descriptor, image_state > &state,
    const graph_type::vertex_descriptor &v
  ) {
    unsigned int score = 0;
    if( (*graph)[ v ].create_info ) {
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
            is->second.mode == image_mode::readable_after_sync_writable_after_sync;
          if( !ready ) {
            return std::make_pair( false, 0 );
          }
          if( is->second.mode != image_mode::readable_without_sync_writable_after_sync ) {
            ++score;
          }
        }
      }
      for( const auto &i: (*graph)[ v ].create_info->get_inout() ) {
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
    else if( (*graph)[ v ].fill_create_info ) {
      const auto &o = (*graph)[ v ].fill_create_info->output;
      const auto &is = state.find( std::get< image_pool::image_descriptor >( o ) );
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
    const graph_type::vertex_descriptor &v
  ) {
    shader_graph_command_list list;
    if( (*graph)[ v ].create_info ) {
      for( const auto &i: (*graph)[ v ].create_info->get_input() ) {
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
      for( const auto &i: (*graph)[ v ].create_info->get_inout() ) {
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
      for( const auto &o: (*graph)[ v ].create_info->get_plan().output ) {
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
              break;
            }
          }
        }
      }
      list.push_back(
        std::make_shared< image_io >(
          *(*graph)[ v ].create_info
        )
      );
    }
    else if( (*graph)[ v ].fill_create_info ) {
      const auto &o = (*graph)[ v ].fill_create_info->output;
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
        is->second.last_generator_type = image_generator_type::output;
        const auto [out_begin,out_end] = out_edges( v, *graph );
        for( const auto &edge: boost::make_iterator_range( out_begin, out_end ) ) {
          const auto &sub = (*graph)[ edge ];
          const auto match = std::find_if(
            sub.begin(),
            sub.end(),
            [&]( const auto &e ) {
              return e.from == (*graph)[ v ].fill_create_info->name;
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
        std::make_shared< image_io >( /////
          *(*graph)[ v ].create_info
        )
      );
    }
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
        rec.barrier( std::get< shader_graph_barrier >( c ).state );
      }
    }
  }
}

