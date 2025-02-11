#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/gbuffer.hpp>
#include <gct/graphics.hpp>
#include <gct/compute.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/load_obj_particle.hpp>
#include <gct/draw_particle.hpp>
#include <gct/particle_buffer.hpp>

namespace gct {

draw_particle::draw_particle(
  const draw_particle_create_info &ci
) : props( ci ) {
  static const auto stencil_op = vk::StencilOpState()
    .setCompareOp( vk::CompareOp::eAlways )
    .setFailOp( vk::StencilOp::eKeep )
    .setPassOp( vk::StencilOp::eKeep );
  
  auto cbsci = pipeline_color_blend_state_create_info_t();
  for( unsigned int i = 0u; i != props.output->get_props().color_buffer_count; ++i ) {
    cbsci
      .add_attachment(
        vk::PipelineColorBlendAttachmentState()
          .setBlendEnable( true )
          .setColorBlendOp( vk::BlendOp::eAdd )
          .setSrcColorBlendFactor( vk::BlendFactor::eOne )
          .setDstColorBlendFactor( vk::BlendFactor::eOne )
          .setAlphaBlendOp( vk::BlendOp::eAdd )
          .setSrcAlphaBlendFactor( vk::BlendFactor::eOne )
          .setDstAlphaBlendFactor( vk::BlendFactor::eOne )
          .setColorWriteMask(
            vk::ColorComponentFlagBits::eR|
            vk::ColorComponentFlagBits::eG|
            vk::ColorComponentFlagBits::eB|
            vk::ColorComponentFlagBits::eA
          )
      );
  }
  draw.reset( new graphics(
    graphics_create_info()
      .set_allocator_set( props.allocator_set )
      .set_pipeline_create_info(
        graphics_pipeline_create_info_t()
          .set_input_assembly(
            pipeline_input_assembly_state_create_info_t()
              .set_basic(
                vk::PipelineInputAssemblyStateCreateInfo()
                  .setTopology( vk::PrimitiveTopology::ePointList )
              )
          )
          .set_viewport(
            pipeline_viewport_state_create_info_t()
              .add_size( props.output->get_props().width, props.output->get_props().height )
          )
          .set_depth_stencil(
            pipeline_depth_stencil_state_create_info_t()
              .set_basic(
                vk::PipelineDepthStencilStateCreateInfo()
                  .setDepthTestEnable( true )
                  .setDepthWriteEnable( false )
                  .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
                  .setDepthBoundsTestEnable( false )
                  .setStencilTestEnable( false )
                  .setFront( stencil_op )
                  .setBack( stencil_op )
              )
          )
          .set_color_blend( cbsci )
          .set_render_pass( props.output->get_render_pass(), 0 )
      )
      .set_swapchain_image_count( props.output->get_props().swapchain_image_count )
      .add_shader( props.shaders )
      .set_resources( props.resources )
  ) );

  vamap = draw->get_vamap();
  stride = draw->get_stride();
}

void draw_particle::operator()(
  command_buffer_recorder_t &rec,
  unsigned int image_index,
  const particle_buffer &pb
) const {
  if( props.output->get_props().clear_color[ 3 ] < 1.0 ) {
    props.output->clear_color( rec, image_index );
  }
  {
    auto render_pass_token = rec.begin_render_pass(
      props.output->get_render_pass_begin_info( image_index ),
      vk::SubpassContents::eInline
    );
    rec.bind_vertex_buffer( pb.get_buffer() );
    draw->bind( rec, image_index );
    rec->draw( pb.get_count(), 1u, 0u, 0u );
  }
}

}

