#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/compute.hpp>
#include <gct/hbao2.hpp>
#include <gct/exception.hpp>

namespace gct {
hbao2::hbao2(
  const hbao2_create_info &ci
) :
  property_type( ci ) {
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "hbao2::hbao2", p0_path, "0" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "hbao2::hbao2", p45_path, "45" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "hbao2::hbao2", p90_path, "90" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "hbao2::hbao2", p135_path, "135" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "hbao2::hbao2", mix_path, "mix" )
  
  p0 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( p0_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  p45 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( p45_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  p90 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( p90_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  p135 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( p135_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  mix = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( mix_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
}
shader_graph::vertex::combined_result_type hbao2::operator()(
  shader_graph::builder &b,
  const shader_graph::vertex::subresult_type &input
) const {
  std::string node_prefix;
  if( props.node_name.empty() ) {
    node_prefix = "hbao2." + std::to_string( std::intptr_t( this ) ) + ".";
  }
  else {
    node_prefix = "hbao2." + props.node_name + ".";
  }
  LIBGCT_SHADER_SET_RENAME( rename, "src", src_name )
  LIBGCT_SHADER_SET_RENAME( rename, "dest", dest_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial0", partial0_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial45", partial45_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial90", partial90_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial135", partial135_name )

  auto r0 = b.call(
    b.get_image_io_create_info(
      p0,
      gct::image_io_plan()
        .add_input( src_name )
        .add_output( dest_name, src_name, glm::vec2( 1.f, -1.f ), props.format )
        .set_dim( src_name, glm::vec2( 1.f, -1.f ) )
        .set_node_name( node_prefix + "0" )
    )
    .set_push_constant( "unproject", *props.unproject )
  )( input );
  auto r45 = b.call(
    b.get_image_io_create_info(
      p45,
      gct::image_io_plan()
        .add_input( src_name )
        .add_output( dest_name, src_name, glm::vec2( 1.f, -1.f ), props.format )
        .set_dim( src_name, glm::vec2( 1.f, -1.f ) )
        .set_node_name( node_prefix + "45" )
    )
    .set_push_constant( "unproject", *props.unproject )
  )( input );
  auto r90 = b.call(
    b.get_image_io_create_info(
      p90,
      gct::image_io_plan()
        .add_input( src_name )
        .add_output( dest_name, src_name, glm::vec2( 1.f, -1.f ), props.format )
        .set_dim( src_name, glm::vec2( 1.f, -1.f ) )
        .set_node_name( node_prefix + "90" )
    )
    .set_push_constant( "unproject", *props.unproject )
  )( input );
  auto r135 = b.call(
    b.get_image_io_create_info(
      p135,
      gct::image_io_plan()
        .add_input( src_name )
        .add_output( dest_name, src_name, glm::vec2( 1.f, -1.f ), props.format )
        .set_dim( src_name, glm::vec2( 1.f, -1.f ) )
        .set_node_name( node_prefix + "135" )
    )
    .set_push_constant( "unproject", *props.unproject )
  )( input );
  auto rmix = b.call(
    b.get_image_io_create_info(
      mix,
      gct::image_io_plan()
        .add_input( partial0_name )
        .add_input( partial45_name )
        .add_input( partial90_name )
        .add_input( partial135_name )
        .add_output( dest_name, partial0_name, 1.f )
        .set_dim( partial0_name )
        .set_node_name( node_prefix + "mix" )
    )
  )( {
     { partial0_name, r0 },
     { partial45_name, r45 },
     { partial90_name, r90 },
     { partial135_name, r135 }
  } );
  return
    shader_graph::vertex::combined_result_type()
      .add( partial0_name, r0 )
      .add( partial45_name, r45 )
      .add( partial90_name, r90 )
      .add( partial135_name, r135 )
      .add( dest_name, rmix );
}

}

