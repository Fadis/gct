#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/compute.hpp>
#include <gct/skyview_froxel2.hpp>
#include <gct/exception.hpp>
#include <gct/scene_graph_resource.hpp>
#include <gct/sampler_pool.hpp>
#include <gct/get_library_path.hpp>

namespace gct {
skyview_froxel2::skyview_froxel2(
  const skyview_froxel2_create_info &ci
) :
  property_type( ci ) {
  LIBGCT_SHADER_SET_USE_SYSTEM_SHADER_IF_NOT_EXIST( "skyview" );
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "skyview_froxel2::skyview_froxel2", generate_path, "generate" )
  LIBGCT_SHADER_SET_VALIDATE( shader_set, "skyview_froxel2::skyview_froxel2", render_path, "render" )
  if( !get_props().sigma ) {
    throw exception::invalid_argument( "skyview_froxel2::skyview_froxel2 : sigma is not set", __FILE__, __LINE__ );
  }

  generate = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( generate_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
  render = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( render_path )
      .set_resources( props.resources )
      .set_scene_graph( props.scene_graph )
  );
}
shader_graph::vertex::combined_result_type skyview_froxel2::operator()(
  shader_graph::builder &b,
  const shader_graph::vertex::subresult_type &gbuffer,
  const shader_graph::vertex::subresult_type &depth,
  const shader_graph::vertex::subresult_type &transmittance
) const {
  std::string node_prefix;
  if( props.node_name.empty() ) {
    node_prefix = "skyview_froxel2." + std::to_string( std::intptr_t( this ) ) + ".";
  }
  else {
    node_prefix = "skyview_froxel2." + props.node_name + ".";
  }
  LIBGCT_SHADER_SET_RENAME( rename, "gbuffer", gbuffer_name )
  LIBGCT_SHADER_SET_RENAME( rename, "depth", depth_name )
  LIBGCT_SHADER_SET_RENAME( rename, "transmittance", transmittance_name )
  LIBGCT_SHADER_SET_RENAME( rename, "froxel", froxel_name )
  LIBGCT_SHADER_SET_RENAME( rename, "dest", dest_name )

  const auto linear_sampler = get_props().scene_graph->sampler->allocate_linear();

  auto generate_result = b.call(
    b.get_image_io_create_info(
      generate,
      gct::image_io_plan()
        .add_sampled( transmittance_name, linear_sampler )
        .add_output(
          froxel_name,
          image_allocate_info()
            .set_create_info(
              image_create_info_t()
                .set_basic(
                  vk::ImageCreateInfo()
                    .setExtent(
                      vk::Extent3D()
                        .setWidth( props.froxel_xy_resolution )
                        .setHeight( props.froxel_xy_resolution )
                        .setDepth( props.froxel_z_resolution )
                    )
                    .setTiling( vk::ImageTiling::eOptimal )
                    .setImageType( vk::ImageType::e3D )
                    .setUsage(
                      vk::ImageUsageFlagBits::eStorage |
                      vk::ImageUsageFlagBits::eSampled
                    )
                    .setFormat( vk::Format::eR16G16B16A16Sfloat )
                    .setSamples( vk::SampleCountFlagBits::e1 )
                    .setMipLevels( 1u )
                    .setArrayLayers( 1u )
                    .setSharingMode( vk::SharingMode::eExclusive )
                    .setInitialLayout( vk::ImageLayout::eUndefined )
                
                )
            )
        )
        .set_dim( froxel_name )
        .set_node_name( node_prefix + "generate" )
    )
    .set_push_constant( "sigma", *get_props().sigma )
    .set_push_constant( "g", get_props().g )
    .set_push_constant( "ground_radius", get_props().ground_radius )
    .set_push_constant( "top_radius", get_props().top_radius )
    .set_push_constant( "altitude", get_props().altitude )
  )(
    shader_graph::vertex::combined_result_type()
      .add( transmittance_name, transmittance )
  );
  auto render_result = b.call(
    b.get_image_io_create_info(
      render,
      gct::image_io_plan()
        .add_input( gbuffer_name )
        .add_input( depth_name )
        .add_sampled( froxel_name, linear_sampler )
        .add_output( dest_name, gbuffer_name, glm::vec2( props.scale, 1.0f/8.0f ), vk::Format::eR16G16B16A16Sfloat )
        .set_dim( gbuffer_name, glm::vec2( props.scale, 1.0f/8.0f ) )
        .set_node_name( node_prefix + "render" )
    )
    .set_push_constant( "world_to_screen", *props.world_to_screen )
    .set_push_constant( "unproject", *props.unproject_to_world )
  )(
    shader_graph::vertex::combined_result_type()
      .add( gbuffer_name, gbuffer )
      .add( depth_name, depth )
      .add( froxel_name, generate_result )
  );
  return
    shader_graph::vertex::combined_result_type()
      .add( froxel_name, generate_result )
      .add( dest_name, render_result );
}

shader_graph::vertex::combined_result_type skyview_froxel2::operator()(
  shader_graph::builder &b,
  const shader_graph::vertex::subresult_type &gbuffer,
  const shader_graph::vertex::subresult_type &position,
  const shader_graph::vertex::subresult_type &start,
  const shader_graph::vertex::subresult_type &next,
  const shader_graph::vertex::subresult_type &transmittance,
  const buffer_pool::buffer_descriptor &ppll_state_id
) const {
  std::string node_prefix;
  if( props.node_name.empty() ) {
    node_prefix = "skyview_froxel2." + std::to_string( std::intptr_t( this ) ) + ".";
  }
  else {
    node_prefix = "skyview_froxel2." + props.node_name + ".";
  }
  LIBGCT_SHADER_SET_RENAME( rename, "gbuffer", gbuffer_name )
  LIBGCT_SHADER_SET_RENAME( rename, "position", position_name )
  LIBGCT_SHADER_SET_RENAME( rename, "transmittance", transmittance_name )
  LIBGCT_SHADER_SET_RENAME( rename, "froxel", froxel_name )
  LIBGCT_SHADER_SET_RENAME( rename, "dest", dest_name )
  LIBGCT_SHADER_SET_RENAME( rename, "start", start_name )
  LIBGCT_SHADER_SET_RENAME( rename, "next", next_name )

  const auto linear_sampler = get_props().scene_graph->sampler->allocate_linear();

  auto generate_result = b.call(
    b.get_image_io_create_info(
      generate,
      gct::image_io_plan()
        .add_sampled( transmittance_name, linear_sampler )
        .add_output(
          froxel_name,
          image_allocate_info()
            .set_create_info(
              image_create_info_t()
                .set_basic(
                  vk::ImageCreateInfo()
                    .setExtent(
                      vk::Extent3D()
                        .setWidth( props.froxel_xy_resolution )
                        .setHeight( props.froxel_xy_resolution )
                        .setDepth( props.froxel_z_resolution )
                    )
                    .setTiling( vk::ImageTiling::eOptimal )
                    .setImageType( vk::ImageType::e3D )
                    .setUsage(
                      vk::ImageUsageFlagBits::eStorage |
                      vk::ImageUsageFlagBits::eSampled
                    )
                    .setFormat( vk::Format::eR16G16B16A16Sfloat )
                    .setSamples( vk::SampleCountFlagBits::e1 )
                    .setMipLevels( 1u )
                    .setArrayLayers( 1u )
                    .setSharingMode( vk::SharingMode::eExclusive )
                    .setInitialLayout( vk::ImageLayout::eUndefined )
                
                )
            )
        )
        .set_dim( froxel_name )
        .set_node_name( node_prefix + "generate" )
    )
    .set_push_constant( "sigma", *get_props().sigma )
    .set_push_constant( "g", get_props().g )
    .set_push_constant( "ground_radius", get_props().ground_radius )
    .set_push_constant( "top_radius", get_props().top_radius )
    .set_push_constant( "altitude", get_props().altitude )
  )(
    shader_graph::vertex::combined_result_type()
      .add( transmittance_name, transmittance )
  );
  auto render_result = b.call(
    b.get_image_io_create_info(
      render,
      gct::image_io_plan()
        .add_input( gbuffer_name )
        .add_input( position_name )
        .add_input( start_name )
        .add_input( next_name )
        .add_sampled( froxel_name, linear_sampler )
        .add_output( dest_name, gbuffer_name, glm::vec2( props.scale, 1.0f/8.0f ), vk::Format::eR16G16B16A16Sfloat )
        .set_dim( gbuffer_name, glm::vec2( props.scale, 1.0f/8.0f ) )
        .set_node_name( node_prefix + "render" )
    )
    .set_push_constant( "world_to_screen", *props.world_to_screen )
    .set_push_constant( "unproject", *props.unproject_to_world )
    .set_push_constant( "ppll_state_id", *ppll_state_id )
  )(
    shader_graph::vertex::combined_result_type()
      .add( gbuffer_name, gbuffer )
      .add( position_name, position )
      .add( start_name, start )
      .add( next_name, next )
      .add( froxel_name, generate_result )
  );
  return
    shader_graph::vertex::combined_result_type()
      .add( froxel_name, generate_result )
      .add( dest_name, render_result );
}


}

