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
#include <gct/draw_line.hpp>
#include <vulkan/vulkan_enums.hpp>

namespace gct {

draw_line::draw_line(
  const draw_line_create_info &ci
) : props( ci ) {
  output = std::make_shared< gct::gbuffer >(
    gct::gbuffer_create_info()
      .set_allocator( props.allocator_set.allocator )
      .set_width( props.width )
      .set_height( props.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( props.color_attachment_count )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
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
  auto cbsci = pipeline_color_blend_state_create_info_t();
  const auto &attachments = output->get_render_pass()->get_props().get_attachment();
  for( const auto &attachment: attachments ) {
    if( format_to_aspect( attachment.format ) == vk::ImageAspectFlagBits::eColor ) {
      cbsci
        .add_attachment(
          vk::PipelineColorBlendAttachmentState()
            .setBlendEnable( VK_FALSE )
            .setColorWriteMask(
              vk::ColorComponentFlagBits::eR |
              vk::ColorComponentFlagBits::eG |
              vk::ColorComponentFlagBits::eB |
              vk::ColorComponentFlagBits::eA
            )
        );
    }
  }
  pipeline_depth_stencil_state_create_info_t dssci;
  if( std::find_if(
    attachments.begin(),
    attachments.end(),
    []( const auto &attachment ) {
      return format_to_aspect( attachment.format ) & vk::ImageAspectFlagBits::eDepth;
    }
  ) != attachments.end() ) {
    dssci
      .set_basic(
        vk::PipelineDepthStencilStateCreateInfo()
          .setDepthTestEnable( VK_FALSE )
          .setDepthWriteEnable( VK_FALSE )
          .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
          .setDepthBoundsTestEnable( VK_FALSE )
          .setStencilTestEnable( VK_FALSE )
          .setFront( stencil_op )
          .setBack( stencil_op )
      );
  }
  else {
    dssci
      .set_basic(
        vk::PipelineDepthStencilStateCreateInfo()
          .setDepthTestEnable( VK_FALSE )
          .setDepthWriteEnable( VK_FALSE )
          .setDepthCompareOp( vk::CompareOp::eLessOrEqual )
          .setDepthBoundsTestEnable( VK_FALSE )
          .setStencilTestEnable( VK_FALSE )
          .setFront( stencil_op )
          .setBack( stencil_op )
      );
  }
  pipeline = std::make_shared< graphics >(
    graphics_create_info()
      .set_allocator_set( props.allocator_set )
      .set_pipeline_create_info(
        graphics_pipeline_create_info_t()
          .set_vertex_input( vertex_input )
          .set_input_assembly(
            pipeline_input_assembly_state_create_info_t()
              .set_basic(
                vk::PipelineInputAssemblyStateCreateInfo()
                  .setTopology( vk::PrimitiveTopology::eLineList )
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
                  .setLineWidth( props.line_width )
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
          .set_dynamic(
            pipeline_dynamic_state_create_info_t()
          )
          .set_render_pass( output->get_render_pass(), 0u )
          .rebuild_chain()
      )
      .set_swapchain_image_count( 1u )
      .add_shader( props.shaders )
      .set_resources( props.resources )
  );
  vertex_buffer = props.allocator_set.allocator->create_mappable_buffer(
    sizeof( glm::vec4 ) * props.max_line_count * 2u, vk::BufferUsageFlagBits::eVertexBuffer
  );
}
void draw_line::push( const glm::vec3 &begin, const glm::vec3 &end ) {
  pushed.push_back( glm::vec4( begin, 1.f ) );
  pushed.push_back( glm::vec4( end, 1.f ) );
  dirty = true;
}
void draw_line::operator()(
  command_buffer_recorder_t &rec,
  const glm::mat4 &matrix
) {
  if( dirty ) {
    {
      auto mapped = vertex_buffer->map< glm::vec4 >();
      std::copy( pushed.begin(), pushed.end(), mapped.begin() );
    }
    rec.sync_to_device( vertex_buffer );
    rec.barrier( { vertex_buffer->get_buffer() }, {} );
    dirty = false;
  }
  {
    auto render_pass_token = rec.begin_render_pass(
      output->get_render_pass_begin_info( 0 ),
      vk::SubpassContents::eInline
    );
    rec->bindVertexBuffers( 0u, { **vertex_buffer }, { 0u } );
    pipeline->bind( rec, 0 );
    {
      const auto pc = push_constant_type()
        .set_matrix( matrix );
      rec->pushConstants(
        **pipeline->get_layout(),
        pipeline->get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        0u,
        sizeof( push_constant_type ),
        &pc
      );
    }
    rec->draw( pushed.size(), 1u, 0u, 0u );
  }
}
void draw_line::reset() {
  pushed.clear();
  dirty = true;
}
const std::shared_ptr< image_view_t > &draw_line::get_image_view() const {
  return output->get_image_view( 0u );
}

}

