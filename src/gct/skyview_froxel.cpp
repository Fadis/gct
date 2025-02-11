#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/compute.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/skyview_froxel.hpp>

namespace gct {

skyview_froxel::skyview_froxel(
  const skyview_froxel_create_info &ci
) : props( ci ) {
  froxel = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_3d_image(
          props.froxel_xy_resolution,
          props.froxel_xy_resolution,
          props.froxel_z_resolution
        )
        .setUsage(
          vk::ImageUsageFlagBits::eStorage|
          vk::ImageUsageFlagBits::eSampled
        )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( props.width, props.height )
          .setUsage(
           vk::ImageUsageFlagBits::eStorage|
           vk::ImageUsageFlagBits::eSampled
         )
      );

  rendered = props.allocator_set.allocator->create_image_view(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  auto linear_sampler = get_device( *props.allocator_set.allocator ).get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  state = props.allocator_set.allocator->create_mappable_buffer(
    sizeof( skyview_froxel_param ),
    vk::BufferUsageFlagBits::eUniformBuffer
  );

  generate = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.generate_shader )
      .set_swapchain_image_count( 1u )
      .add_resource( { props.froxel_state_name, state } )
      .add_resource( { props.transmittance_name, linear_sampler, props.transmittance, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { props.multiscat_name, linear_sampler, props.multiscat, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { props.froxel_name, froxel, vk::ImageLayout::eGeneral } )
  );

  render = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.render_shader )
      .set_swapchain_image_count( 1u )
      .add_resource( { props.froxel_state_name, state } )
      .add_resource( { props.gbuffer_name, props.gbuffer, vk::ImageLayout::eGeneral } )
      .add_resource( { props.froxel_name, linear_sampler, froxel, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { props.output_name, rendered, vk::ImageLayout::eGeneral } )
  );
}
void skyview_froxel::operator()(
  command_buffer_recorder_t &rec,
  const skyview_froxel_param &param
) const {
  if( froxel->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( froxel, vk::ImageLayout::eGeneral );
  }
  if( rendered->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( rendered, vk::ImageLayout::eGeneral );
  }

  rec.copy(
    param,
    state
  );

  rec.convert_image(
    froxel->get_factory(),
    vk::ImageLayout::eGeneral
  );
      
  (*generate)(
    rec,
    0,
    props.froxel_xy_resolution,
    props.froxel_xy_resolution,
    props.froxel_z_resolution
  );

  rec.convert_image(
    froxel->get_factory(),
    vk::ImageLayout::eShaderReadOnlyOptimal
  );
      
  (*render)(
    rec,
    0,
    props.width,
    props.height,
    1u
  );
}

}

