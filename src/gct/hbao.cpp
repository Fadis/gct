#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/buffer.hpp>
#include <gct/compute.hpp>
#include <gct/hbao.hpp>

namespace gct {
hbao::hbao(
  const hbao_create_info &ci
) :
  property_type( ci ),
  width(
    ci.input[ 0 ]->get_factory()->get_props().get_basic().extent.width
  ),
  height(
    ci.input[ 0 ]->get_factory()->get_props().get_basic().extent.height
  )
{
  if( props.temp.empty() ) {
    props.temp = props.allocator_set.allocator->create_image_views(
      gct::image_create_info_t()
        .set_basic(
          gct::basic_2d_image( width, height )
            .setFormat( vk::Format::eR32G32Sfloat )
            .setArrayLayers( 4 )
            .setUsage( vk::ImageUsageFlagBits::eStorage )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY,
      props.input.size()
    );
  }
  if( props.output.empty() ) {
    props.output = props.allocator_set.allocator->create_image_views(
      gct::image_create_info_t()
        .set_basic(
          gct::basic_2d_image( width, height )
            .setFormat( vk::Format::eR32Sfloat )
            .setArrayLayers( 1 )
            .setUsage( vk::ImageUsageFlagBits::eStorage )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY,
      props.input.size()
    );
  }

  comp.push_back( gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.ao0_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { props.input_name, props.input, vk::ImageLayout::eGeneral } )
      .add_resource( { props.temp_name, props.temp, vk::ImageLayout::eGeneral } )
  ) );
  comp.push_back( gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.ao45_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { props.input_name, props.input, vk::ImageLayout::eGeneral } )
      .add_resource( { props.temp_name, props.temp, vk::ImageLayout::eGeneral } )
  ) );
  comp.push_back( gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.ao90_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { props.input_name, props.input, vk::ImageLayout::eGeneral } )
      .add_resource( { props.temp_name, props.temp, vk::ImageLayout::eGeneral } )
  ) );
  comp.push_back( gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.ao135_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { props.input_name, props.input, vk::ImageLayout::eGeneral } )
      .add_resource( { props.temp_name, props.temp, vk::ImageLayout::eGeneral } )
  ) );
  comp.push_back( gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.sum_shader )
      .set_swapchain_image_count( props.input.size() )
      .set_resources( props.resources )
      .add_resource( { props.temp_name, props.temp, vk::ImageLayout::eGeneral } )
      .add_resource( { props.output_name, props.output, vk::ImageLayout::eGeneral } )
  ) );
}
void hbao::operator()(
  command_buffer_recorder_t &rec,
  std::uint32_t image_index
) const {
  if( props.input[ image_index ]->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( props.input[ image_index ], vk::ImageLayout::eGeneral );
  }
  if( props.temp[ image_index ]->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( props.temp[ image_index ], vk::ImageLayout::eGeneral );
  }
  if( props.output[ image_index ]->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( props.output[ image_index ], vk::ImageLayout::eGeneral );
  }
  for( unsigned int i = 0u; i != 4u; ++i ) {
    comp[ i ]( rec, image_index, width, height, 1 );
  }
  rec.compute_barrier(
    {},
    { props.temp[ image_index ]->get_factory() }
  );
  comp[ 4u ]( rec, image_index, width, height, 1 );
}

}

