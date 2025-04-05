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
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", v0_path, "v0" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", v1_path, "v1" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", v2_path, "v2" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", v3_path, "v3" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", v4_path, "v4" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "lrlf_dof::lrlf_dof", mix_path, "mix" )
  
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
  v0 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( v0_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  v1 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( v1_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  v2 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( v2_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  v3 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( v3_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  v4 = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( v4_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  mix = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( mix_path )
      .set_scene_graph( props.scene_graph )
      .set_resources( props.resources )
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
  auto p0 = b.call(
    b.get_image_io_create_info(
      v0,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, t0 )
      .add( coc_name, coc )
  );
  auto p1 = b.call(
    b.get_image_io_create_info(
      v1,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, t1 )
      .add( coc_name, coc )
  );
  auto p2 = b.call(
    b.get_image_io_create_info(
      v2,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, t2 )
      .add( coc_name, coc )
  );
  auto p3 = b.call(
    b.get_image_io_create_info(
      v3,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, t3 )
      .add( coc_name, coc )
  );
  auto p4 = b.call(
    b.get_image_io_create_info(
      v4,
      gct::image_io_plan()
        .add_input( src_name )
        .add_input( coc_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( src_name, t4 )
      .add( coc_name, coc )
  );
  auto mixed = b.call(
    b.get_image_io_create_info(
      mix,
      gct::image_io_plan()
        .add_input( partial0_name )
        .add_input( partial1_name )
        .add_input( partial2_name )
        .add_input( partial3_name )
        .add_input( partial4_name )
        .add_output( dest_name, src_name, 1.f )
        .set_dim( src_name )
    )
  )(
    shader_graph::vertex::combined_result_type()
      .add( partial0_name, p0 )
      .add( partial1_name, p1 )
      .add( partial2_name, p2 )
      .add( partial3_name, p3 )
      .add( partial4_name, p4 )
  );
  return mixed[ dest_name ];
}

}

