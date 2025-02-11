#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image_view.hpp>
#include <gct/image.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/distance_field.hpp>
#include <gct/device.hpp>
#include <gct/compute.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>

namespace gct {

distance_field::distance_field(
  const distance_field_create_info &ci
) :
  property_type( ci ),
  working_image(
    ci.allocator_set.allocator,
    ci.size_factor,
    vk::Format::eR32Uint
  ),
  distance_field_image(
    ci.allocator_set.allocator,
    ci.size_factor,
    vk::Format::eR32Sfloat
  ) {

  clear_.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.clear_shader )
      .set_swapchain_image_count( 1 )
      .add_resource( { "dest_image", working_image.get_image(), vk::ImageLayout::eGeneral } )
  ) );

  voronoi.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.voronoi_shader )
      .set_swapchain_image_count( 1 )
      .add_resource( { "dest_image", working_image.get_image(), vk::ImageLayout::eGeneral } )
  ) );

  distance_field_.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.distance_field_shader )
      .set_swapchain_image_count( 1 )
      .add_resource( { "src_image", working_image.get_image(), vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image", distance_field_image.get_image(), vk::ImageLayout::eGeneral } )
  ) );
}
void distance_field::clear(
  command_buffer_recorder_t &rec
) const {
  const unsigned int image_size = 1u << props.size_factor;
  if( working_image.get_image()->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( working_image.get_image(), vk::ImageLayout::eGeneral );
  }
  if( distance_field_image.get_image()->get_factory()->get_layout().get_uniform_layout() != vk::ImageLayout::eGeneral ) {
    rec.set_image_layout( distance_field_image.get_image(), vk::ImageLayout::eGeneral );
  }
  (*clear_)( rec, 0u, image_size, image_size, image_size );
  rec.barrier(
    vk::AccessFlagBits::eShaderWrite,
    vk::AccessFlagBits::eShaderRead,
    vk::PipelineStageFlagBits::eComputeShader,
    vk::PipelineStageFlagBits::eFragmentShader,
    vk::DependencyFlagBits( 0 ),
    {},
    { working_image.get_image()->get_factory() }
  );
}
void distance_field::generate(
  command_buffer_recorder_t &rec
) const {
  const unsigned int image_size = 1u << props.size_factor;
  for( unsigned int step = image_size / 2u; step != 0u; step >>= 1 ) {
    rec->pushConstants(
      **voronoi->get_pipeline()->get_props().get_layout(),
      vk::ShaderStageFlagBits::eCompute,
      0,
      sizeof( unsigned int ),
      &step
    );
    (*voronoi)( rec, 0u, image_size, image_size, image_size * 26 );
    rec.compute_barrier(
      {},
      { working_image.get_image()->get_factory() }
    );
  }
  {
    (*distance_field_)( rec, 0u, image_size, image_size, image_size );
  }
}

}

