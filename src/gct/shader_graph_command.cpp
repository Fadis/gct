#include <nlohmann/json.hpp>
#include <gct/image_io.hpp>
#include <gct/image_fill_create_info.hpp>
#include <gct/image_blit_create_info.hpp>
#include <gct/shader_graph_command.hpp>

namespace gct {

void to_json( nlohmann::json &dest, const shader_graph_command &src ) {
  dest = nlohmann::json::object();
  if( shader_graph_command_id( src.index() ) == shader_graph_command_id::call ) {
    dest[ "type" ] = "call";
    dest[ "value" ] = *std::get< std::shared_ptr< image_io > >( src );
  }
  else if( shader_graph_command_id( src.index() ) == shader_graph_command_id::barrier ) {
    dest[ "type" ] = "barrier";
    dest[ "value" ] = std::get< shader_graph_barrier >( src );
  }
  else if( shader_graph_command_id( src.index() ) == shader_graph_command_id::fill ) {
    dest[ "type" ] = "fill";
    dest[ "value" ] = *std::get< std::shared_ptr< image_fill_create_info > >( src );
  }
  else if( shader_graph_command_id( src.index() ) == shader_graph_command_id::blit ) {
    dest[ "type" ] = "blit";
    dest[ "value" ] = *std::get< std::shared_ptr< image_blit_create_info > >( src );
  }
}

void to_json( nlohmann::json &dest, const shader_graph_command_list &src ) {
  dest = nlohmann::json::array();
  for( const auto &v: src ) {
    dest.push_back( v );
  }
}


std::string to_string( const shader_graph_command &src ) {
  std::string dest;
  if( shader_graph_command_id( src.index() ) == shader_graph_command_id::call ) {
    const auto &io = *std::get< std::shared_ptr< image_io > >( src );
    if( !io.get_props().get_plan().node_name.empty() ) {
      dest += "call:";
      dest += io.get_props().get_plan().node_name;
      dest += "( ";
    }
    else {
      dest += "call( ";
    }
    bool initial = true;
    for( const auto &i: io.get_props().get_input() ) {
      if( initial ) initial = false;
      else dest += ", ";
      dest += "in " + i.first + ":" + std::to_string( *i.second );
    }
    for( const auto &i: io.get_props().get_inout() ) {
      if( initial ) initial = false;
      else dest += ", ";
      dest += "inout " + i.first + ":" + std::to_string( *i.second );
    }
    for( const auto &i: io.get_props().get_sampled() ) {
      if( initial ) initial = false;
      else dest += ", ";
      dest += "sampled " + i.first + ":" + std::to_string( *i.second );
    }
    dest += " ) -> ( ";
    initial = true;
    for( const auto &i: io.get_props().get_plan().output ) {
      if( initial ) initial = false;
      else dest += ", ";
      dest += "out " + i.first + ":" + std::to_string( *std::get< image_pool::image_descriptor >( i.second ) );
    }
    for( const auto &i: io.get_props().get_inout() ) {
      if( initial ) initial = false;
      else dest += ", ";
      dest += "inout " + i.first + ":" + std::to_string( *i.second );
    }
    dest += " );\n";
  }
  else if( shader_graph_command_id( src.index() ) == shader_graph_command_id::barrier ) {
    const auto &b = std::get< shader_graph_barrier >( src );
    dest += "barrier< " + nlohmann::json( b.state.config ).dump() + " >( ";
    bool initial = true;
    for( const auto &v: b.view ) {
      if( initial ) initial = false;
      else dest += ", ";
      dest += std::to_string( *v );
    }
    dest += " );\n";
  }
  else if( shader_graph_command_id( src.index() ) == shader_graph_command_id::fill ) {
    const auto &fill = *std::get< std::shared_ptr< image_fill_create_info > >( src );
    if( !fill.node_name.empty() ) {
      dest += "fill:";
      dest += fill.node_name;
      dest += "( ";
    }
    else {
      dest += "fill( ";
    }
    dest += "color:( ";
    dest += std::to_string( fill.color[ 0 ] ) + ", ";
    dest += std::to_string( fill.color[ 1 ] ) + ", ";
    dest += std::to_string( fill.color[ 2 ] ) +  ", ";
    dest += std::to_string( fill.color[ 3 ] ) + " ),";
    dest += " independent:";
    dest += ( fill.independent ? "true" :"false" ) + std::string( " ) -> ( " );
    dest += fill.name + ":";
    dest += std::to_string( *std::get< image_pool::image_descriptor >( fill.output ) );
    dest += " );\n";
  }
  else if( shader_graph_command_id( src.index() ) == shader_graph_command_id::blit ) {
    const auto &blit = *std::get< std::shared_ptr< image_blit_create_info > >( src );
    if( !blit.node_name.empty() ) {
      dest += "blit:";
      dest += blit.node_name;
      dest += "( ";
    }
    else {
      dest += "blit( ";
    }
    dest += blit.input_name + ":";
    dest += std::to_string( *blit.input );
    dest += ", independent:";
    dest += ( blit.independent ? "true" :"false" ) + std::string( " ) -> ( " );
    dest += blit.output_name + ":";
    dest += std::to_string( *std::get< image_pool::image_descriptor >( blit.output ) );
    dest += " );\n";
  }
  return dest;
}

std::string to_string( const shader_graph_command_list &src ) {
  std::string dest;
  for( const auto &v: src ) {
    dest += to_string( v );
  }
  return dest;
}

}

