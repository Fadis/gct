#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/compute.hpp>
#include <gct/lrlf_dof.hpp>
#include <gct/exception.hpp>

namespace gct {
lrlf_dof::lrlf_dof(
  const lrlf_dof_create_info &ci
) :
  property_type( ci ) {
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", h0_path, "h0" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", h1_path, "h1" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", h2_path, "h2" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", h3_path, "h3" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", h4_path, "h4" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", v_path, "v" )
  
  h0 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( h0_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  h1 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( h1_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  h2 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( h2_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  h3 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( h3_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  h4 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( h4_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  v = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( v_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
}
shader_graph::vertex::subresult_type lrlf_dof::operator()(
  shader_graph::builder &b,
  const shader_graph::vertex::subresult_type &input,
  const shader_graph::vertex::subresult_type &coc
) const {
  LIBGCT_SHADER_SET_RENAME( rename, "coc", coc_name )
  LIBGCT_SHADER_SET_RENAME( rename, "src", src_name )
  LIBGCT_SHADER_SET_RENAME( rename, "dest", dest_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial0", partial0_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial1", partial1_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial2", partial2_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial3", partial3_name )
  LIBGCT_SHADER_SET_RENAME( rename, "partial4", partial4_name )

  auto t0 = b.call(
    b.get_image_io_create_info(
      h0,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, input )
      .add( coc_name, coc )
  );
  auto t1 = b.call(
    b.get_image_io_create_info(
      h1,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, input )
      .add( coc_name, coc )
  );
  auto t2 = b.call(
    b.get_image_io_create_info(
      h2,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, input )
      .add( coc_name, coc )
  );
  auto t3 = b.call(
    b.get_image_io_create_info(
      h3,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, input )
      .add( coc_name, coc )
  );
  auto t4 = b.call(
    b.get_image_io_create_info(
      h4,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, input )
      .add( coc_name, coc )
  );
  auto p = b.call(
    b.get_image_io_create_info(
      v,
      gct::image_io_plan()
        .add_input( partial0_name )
        .add_input( partial1_name )
        .add_input( partial2_name )
        .add_input( partial3_name )
        .add_input( partial4_name )
        .add_input( coc_name )
        .add_output( dest_name, partial0_name, 1.f )
        .set_dim( partial0_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( partial0_name, t0 )
      .add( partial1_name, t1 )
      .add( partial2_name, t2 )
      .add( partial3_name, t3 )
      .add( partial4_name, t4 )
      .add( coc_name, coc )
  );
  return p[ dest_name ];
}

}

