#include <gct/allocator.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/buffer.hpp>
#include <gct/compute.hpp>
#include <gct/color.hpp>
#include <gct/rtao.hpp>

namespace gct {
rtao::rtao(
  const rtao_create_info &ci
) :
  property_type( ci ),
  width(
    ci.output->get_factory()->get_props().get_basic().extent.width
  ),
  height(
    ci.output->get_factory()->get_props().get_basic().extent.height
  )
{
  previous_ao = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( width, height )
          .setFormat( ci.output->get_factory()->get_props().get_basic().format )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  previous_history = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( width, height )
          .setFormat( vk::Format::eR32G32Uint )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  
  history = props.allocator_set.allocator->create_image_view(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( width, height )
          .setFormat( vk::Format::eR32G32Uint )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eTransferDst|
            vk::ImageUsageFlagBits::eTransferSrc
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );

  generate_ao.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.rtao_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.ao_name, props.output, vk::ImageLayout::eGeneral } )
      .add_resource( { props.history_name, history, vk::ImageLayout::eGeneral } )
      .add_resource( { props.previous_ao_name, previous_ao, vk::ImageLayout::eGeneral } )
      .add_resource( { props.previous_history_name, previous_history, vk::ImageLayout::eGeneral } )
      .set_ignore_unused_descriptor( true )
  ) );
  
  hgauss.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.hgauss_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.ao_name, props.output, vk::ImageLayout::eGeneral } )
      .add_resource( { props.temporary_name, previous_ao, vk::ImageLayout::eGeneral } )
      .add_resource( { props.history_name, history, vk::ImageLayout::eGeneral } )
      .set_ignore_unused_descriptor( true )
  ) );
  
  vgauss.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.vgauss_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.ao_name, props.output, vk::ImageLayout::eGeneral } )
      .add_resource( { props.temporary_name, previous_ao, vk::ImageLayout::eGeneral } )
      .add_resource( { props.history_name, history, vk::ImageLayout::eGeneral } )
      .set_ignore_unused_descriptor( true )
  ) );

}
void rtao::operator()(
  command_buffer_recorder_t &rec
) const {
  if( previous_ao->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( previous_ao, vk::ImageLayout::eGeneral );
    rec.fill( previous_ao->get_factory(), color::web::black );
  }
  if( previous_history->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( previous_history, vk::ImageLayout::eGeneral );
    rec.fill( previous_history->get_factory(), color::web::black );
  }
  if( props.output->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( props.output, vk::ImageLayout::eGeneral );
  }
  if( history->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( history, vk::ImageLayout::eGeneral );
    rec.fill( history->get_factory(), color::web::black );
  }
  (*generate_ao)( rec, 0u, width, height, 1u );
  rec.compute_barrier(
    {},
    {
      props.output->get_factory(),
      history->get_factory()
    }
  );
  (*hgauss)( rec, 0u, width, height, 1u );
  rec.compute_barrier(
    {},
    {
      previous_ao->get_factory()
    }
  );
  (*vgauss)( rec, 0u, width, height, 1u );
  rec.barrier(
    {},
    {
      props.output->get_factory()
    }
  );
  rec.copy( props.output->get_factory(), previous_ao->get_factory() );
  rec.copy( history->get_factory(), previous_history->get_factory() );
  rec.barrier(
    {},
    {
      previous_ao->get_factory(),
      previous_history->get_factory()
    }
  );
}

}

