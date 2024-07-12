#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/buffer.hpp>
#include <gct/compute.hpp>
#include <gct/color.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/distance_field2.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {
distance_field2::distance_field2(
  const distance_field2_create_info &ci
) :
  props( ci ),
  working_image(
    ci.allocator,
    ci.size_factor,
    vk::Format::eR32Uint,
    1u,
    vk::ImageUsageFlagBits::eStorage|
    vk::ImageUsageFlagBits::eTransferDst
  ),
  distance_field_image(
    ci.allocator,
    ci.size_factor,
    vk::Format::eR16Sfloat
  ) {
  voronoi.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.voronoi_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.working_image_name, working_image.get_image(), vk::ImageLayout::eGeneral } )
  ) );

  distance_field_.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.distance_field_shader )
      .set_swapchain_image_count( 1u )
      .add_resource( { props.working_image_name, working_image.get_image(), vk::ImageLayout::eGeneral } )
      .add_resource( { props.distance_field_name, distance_field_image.get_image(), vk::ImageLayout::eGeneral } )
  ) );
}

void distance_field2::clear(
  command_buffer_recorder_t &rec
) const {
  if( working_image.get_image()->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( working_image.get_image()->get_factory(), vk::ImageLayout::eGeneral );
  }
  if( distance_field_image.get_image()->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( distance_field_image.get_image()->get_factory(), vk::ImageLayout::eGeneral );
  }
  rec.fill( working_image.get_image()->get_factory(), vk::ClearColorValue().setUint32( { 0x40000000u, 0u, 0u, 0 } ) );
  rec.barrier( {}, { working_image.get_image()->get_factory() } );
}
void distance_field2::operator()(
  command_buffer_recorder_t &rec
) const {
  const auto size = 1u << props.size_factor;
  for( std::uint32_t step = size / 2u; step != 0u; step >>= 1 ) {
    rec->pushConstants(
      **voronoi->get_pipeline()->get_props().get_layout(),
      vk::ShaderStageFlagBits::eCompute,
      0,
      sizeof( std::uint32_t ),
      &step
    );
    (*voronoi)( rec, 0u, size, size, size );
    rec.compute_barrier( {}, { working_image.get_image()->get_factory() } );
  }
  (*distance_field_)( rec, 0, size, size, size );
  rec.barrier( {}, { distance_field_image.get_image()->get_factory() } );
}

}

