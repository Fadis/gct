#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/compute.hpp>
#include <gct/gauss.hpp>
#include <gct/exception.hpp>

namespace gct {
gauss::gauss(
  const gauss_create_info &ci
) :
  property_type( ci ) {
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "gauss::gauss", horizontal_path, "horizontal" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "gauss::gauss", vertical_path, "vertical" )
  
  horizontal = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( horizontal_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  vertical = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( vertical_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
}
shader_graph::vertex::combined_result_type gauss::operator()(
  shader_graph::builder &b,
  const shader_graph::vertex::subresult_type &input
) const {
  std::string node_prefix;
  if( props.node_name.empty() ) {
    node_prefix = "gauss." + std::to_string( std::intptr_t( this ) ) + ".";
  }
  else {
    node_prefix = "gauss." + props.node_name + ".";
  }
  LIBGCT_SHADER_SET_RENAME( rename, "src", src_name )
  LIBGCT_SHADER_SET_RENAME( rename, "dest", dest_name )

  auto temp = b.call(
    b.get_image_io_create_info(
      horizontal,
      gct::image_io_plan()
        .add_input( src_name )
        .add_output( dest_name, src_name, glm::vec2( props.scale, 1.0f ) )
        .set_dim( src_name, glm::vec2( props.scale, 1.0f ) )
        .set_node_name( node_prefix + "horizontal" )
    )
  )( input );
  auto output = b.call(
    b.get_image_io_create_info(
      vertical,
      gct::image_io_plan()
        .add_input( src_name )
        .add_output( dest_name, src_name, glm::vec2( 1.f, 1.f ) )
        .set_dim( src_name, glm::vec2( 1.f, 1.f ) )
        .set_node_name( node_prefix + "vertical" )
    )
  )( temp );
  return
    shader_graph::vertex::combined_result_type()
      .add( dest_name, output[ dest_name ] );
}

}

