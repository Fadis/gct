#include <nlohmann/json.hpp>
#include "gct/gbuffer_create_info.hpp"
#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image.hpp>
#include <gct/image_view.hpp>
#include <gct/gbuffer.hpp>
#include <gct/graphics_create_info.hpp>
#include <gct/graphics.hpp>
#include <gct/query_pool.hpp>
#include <gct/format.hpp>
#include <gct/render_pass2.hpp>
#include <gct/render_pass.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/lens_flare.hpp>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>

namespace gct {

lens_flare::lens_flare(
  const lens_flare_create_info &ci
) : props( ci ) {
  output = std::make_shared< gct::gbuffer >(
    gct::gbuffer_create_info()
      .set_allocator( props.allocator_set.allocator )
      .set_width( props.width )
      .set_height( props.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( 1u )
      .set_format( vk::Format::eR32G32B32A32Sfloat )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .disable_depth()
  );

  const auto stencil_op = vk::StencilOpState()
    .setFailOp( vk::StencilOp::eKeep )
    .setPassOp( vk::StencilOp::eKeep )
    .setCompareOp( vk::CompareOp::eAlways );
  
  const auto vertex_input =
    pipeline_vertex_input_state_create_info_t()
      .add_vertex_input_attribute_description(
        vk::VertexInputAttributeDescription()
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
      )
      .add_vertex_input_binding_description(
        vk::VertexInputBindingDescription()
          .setInputRate( vk::VertexInputRate::eVertex )
      );
  auto dsci = pipeline_dynamic_state_create_info_t();

  auto cbsci = pipeline_color_blend_state_create_info_t();
  const auto &attachments = output->get_render_pass()->get_props().get_attachment();
  for( const auto &attachment: attachments ) {
    if( format_to_aspect( attachment.format ) == vk::ImageAspectFlagBits::eColor ) {
      cbsci
        .add_attachment(
          vk::PipelineColorBlendAttachmentState()
            .setBlendEnable( true )
            .setSrcColorBlendFactor( vk::BlendFactor::eOne )
            .setDstColorBlendFactor( vk::BlendFactor::eOne )
            .setColorBlendOp( vk::BlendOp::eAdd )
            .setSrcAlphaBlendFactor( vk::BlendFactor::eOne )
            .setDstAlphaBlendFactor( vk::BlendFactor::eOne )
            .setAlphaBlendOp( vk::BlendOp::eAdd )
            .setColorWriteMask(
              vk::ColorComponentFlagBits::eR|
              vk::ColorComponentFlagBits::eG|
              vk::ColorComponentFlagBits::eB|
              vk::ColorComponentFlagBits::eA
            )
        );
    }
  }
  auto dssci = pipeline_depth_stencil_state_create_info_t()
    .set_basic(
      vk::PipelineDepthStencilStateCreateInfo()
        .setDepthTestEnable( VK_FALSE )
        .setDepthWriteEnable( VK_FALSE )
        .setDepthCompareOp( vk::CompareOp::eAlways )
        .setDepthBoundsTestEnable( VK_FALSE )
        .setStencilTestEnable( VK_FALSE )
        .setFront( stencil_op )
        .setBack( stencil_op )
    );
  pipeline = std::make_shared< graphics >(
    graphics_create_info()
      .set_allocator_set( props.allocator_set )
      .set_descriptor_set_layout( props.descriptor_set_layout )
      .set_external_descriptor_set( props.external_descriptor_set )
      .set_ignore_unused_descriptor( true )
      .set_pipeline_create_info(
        graphics_pipeline_create_info_t()
          .set_vertex_input( vertex_input )
          .set_input_assembly(
            pipeline_input_assembly_state_create_info_t()
              .set_basic(
                vk::PipelineInputAssemblyStateCreateInfo()
                  .setTopology( vk::PrimitiveTopology::ePointList )
              )
          )
          .set_viewport(
            pipeline_viewport_state_create_info_t()
              .add_viewport( output->get_viewport() )
              .add_scissor( output->get_scissor() )
          )
          .set_rasterization(
            pipeline_rasterization_state_create_info_t()
              .set_basic(
                vk::PipelineRasterizationStateCreateInfo()
                  .setDepthClampEnable( VK_FALSE )
                  .setRasterizerDiscardEnable( VK_FALSE )
                  .setPolygonMode( vk::PolygonMode::eFill )
                  .setCullMode( vk::CullModeFlagBits::eNone )
                  .setFrontFace( vk::FrontFace::eCounterClockwise )
                  .setDepthBiasEnable( VK_FALSE )
                  .setLineWidth( 1.0f )
              )
          )
          .set_multisample(
            pipeline_multisample_state_create_info_t()
              .set_basic(
                vk::PipelineMultisampleStateCreateInfo()
              )
          )
          .set_depth_stencil( std::move( dssci ) )
          .set_color_blend( std::move( cbsci ) )
          .set_dynamic( std::move( dsci ) )
          .set_render_pass( output->get_render_pass(), 0u )
          .rebuild_chain()
      )
      .set_swapchain_image_count( 1u )
      .add_shader( props.shaders )
      .set_resources( props.resources )
  );
  point_mesh = props.allocator_set.allocator->create_mappable_buffer(
    sizeof( glm::vec4 ) * props.matrix_count, vk::BufferUsageFlagBits::eVertexBuffer
  );
}
void lens_flare::operator()(
  command_buffer_recorder_t &rec
) {
  auto render_pass_token = rec.begin_render_pass(
    output->get_render_pass_begin_info( 0 ),
    vk::SubpassContents::eInline
  );
  rec->bindVertexBuffers( 0u, { **point_mesh }, { 0u } );
  pipeline->bind( rec, 0 );
  const auto pc = push_constant_type()
    .set_texture_id( *props.texture )
    .set_lens_size( props.lens_size )
    .set_sensor_size( props.sensor_size );
  rec->pushConstants(
    **pipeline->get_layout(),
    pipeline->get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
    0u,
    sizeof( push_constant_type ),
    &pc
  );
  rec->draw( 1u, props.matrix_count, 0u, 0u );
}
const std::shared_ptr< image_view_t > &lens_flare::get_image_view() const {
  return output->get_image_view( 0u );
}

}

