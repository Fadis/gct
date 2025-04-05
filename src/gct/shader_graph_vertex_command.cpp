#include <nlohmann/json.hpp>
#include <gct/image_io_create_info.hpp>
#include <gct/image_fill_create_info.hpp>
#include <gct/image_blit_create_info.hpp>
#include <gct/shader_graph_vertex_command.hpp>

namespace gct::shader_graph {
void to_json( nlohmann::json &dest, const vertex_command &src ) {
  dest = nlohmann::json::object();
  if( vertex_command_id( src.index() ) == vertex_command_id::call ) {
    dest[ "type" ] = "call";
    if( std::get< std::shared_ptr< image_io_create_info > >( src ) ) {
      dest[ "value" ] = *std::get< std::shared_ptr< image_io_create_info > >( src );
    }
    else {
      dest[ "value" ] = nullptr;
    }
  }
  else if( vertex_command_id( src.index() ) == vertex_command_id::fill ) {
    dest[ "type" ] = "fill";
    if( std::get< std::shared_ptr< image_fill_create_info > >( src ) ) {
      dest[ "value" ] = *std::get< std::shared_ptr< image_fill_create_info > >( src );
    }
    else {
      dest[ "value" ] = nullptr;
    }
  }
  else if( vertex_command_id( src.index() ) == vertex_command_id::blit ) {
    dest[ "type" ] = "blit";
    if( std::get< std::shared_ptr< image_blit_create_info > >( src ) ) {
      dest[ "value" ] = *std::get< std::shared_ptr< image_blit_create_info > >( src );
    }
    else {
      dest[ "value" ] = nullptr;
    }
  }
  else if( vertex_command_id( src.index() ) == vertex_command_id::immediate ) {
    dest[ "type" ] = "immediate";
    dest[ "value" ] = *std::get< image_pool::image_descriptor >( src );
  }
}

}

