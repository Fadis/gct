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
#include <gct/occlusion_query.hpp>
#include <vulkan/vulkan_enums.hpp>

namespace gct {

basic_occlusion_query::basic_occlusion_query(
  const occlusion_query_create_info &ci
) : props( ci ) {
  output = std::make_shared< gct::gbuffer >(
    gct::gbuffer_create_info()
      .set_allocator( props.allocator_set.allocator )
      .set_width( props.depth_image->get_factory()->get_props().get_basic().extent.width )
      .set_height( props.depth_image->get_factory()->get_props().get_basic().extent.height )
      .set_layer( 1u )
      .set_swapchain_image_count( 1u )
      .set_color_buffer_count( props.color_attachment_count )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_external_depth( std::vector< std::shared_ptr< image_view_t > >{ props.depth_image } )
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
          .setDepthTestEnable( VK_TRUE )
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
  if( props.dynamic_cull_mode ) {
#if defined(VK_VERSION_1_3)
    dsci
      .add_dynamic_state( vk::DynamicState::eCullMode )
      .add_dynamic_state( vk::DynamicState::eDepthCompareOp );
#elif defined(VK_KHR_VULKAN_EXTENDED_DYNAMIC_EXTENSION_NAME)
    dsci
      .add_dynamic_state( vk::DynamicState::eCullModeExt )
      .add_dynamic_state( vk::DynamicState::eDepthCompareOpExt );
#endif
  }
  aabb_buf = props.allocator_set.allocator->create_mappable_buffer(
    props.max_query_count * sizeof( aabb4 ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );
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
      .add_resource( { "aabb", aabb_buf } )
  );
  if( props.query ) {
    query_pool = get_device( *props.allocator_set.allocator ).get_query_pool(
      gct::query_pool_create_info_t()
        .set_basic(
          vk::QueryPoolCreateInfo()
            .setQueryType( vk::QueryType::eOcclusion )
            .setQueryCount( props.max_query_count )
        )
    );
  }
  point_mesh = props.allocator_set.allocator->create_mappable_buffer(
    sizeof( glm::vec4 ), vk::BufferUsageFlagBits::eVertexBuffer
  );
  if( props.query ) {
    query_pool->reset();
  }
}
occlusion_query_id_t basic_occlusion_query::push( const aabb4 &a ) {
  pushed.push_back( { a } );
  return pushed.size() - 1u;
}
occlusion_query_id_t basic_occlusion_query::push( const std::vector< aabb4 > &a ) {
  pushed.push_back( a );
  return pushed.size() - 1u;
}
void basic_occlusion_query::operator()(
  command_buffer_recorder_t &rec,
  const glm::mat4 &matrix
) {
  if( !transfered ) {
    rec.sync_to_device( point_mesh );
    rec.barrier( { point_mesh->get_buffer() }, {} );
    transfered = true;
  }
  std::vector< std::pair< std::uint32_t, std::uint32_t > > range;
  {
    auto mapped = aabb_buf->map< aabb4 >();
    std::uint32_t head = 0u;
    for( unsigned int i = 0u; i != pushed.size(); ++i ) {
      std::copy(
        pushed[ i ].begin(),
        pushed[ i ].end(),
        std::next( mapped.begin(), head )
      );
      range.push_back( std::make_pair( head, std::uint32_t( pushed[ i ].size() ) ) );
      head += pushed[ i ].size();
    }
  }
  rec.sync_to_device( aabb_buf );
  rec.transfer_to_graphics_barrier( { aabb_buf->get_buffer() }, {} );
  {
    auto render_pass_token = rec.begin_render_pass(
      output->get_render_pass_begin_info( 0 ),
      vk::SubpassContents::eInline
    );
    rec->bindVertexBuffers( 0u, { **point_mesh }, { 0u } );
    pipeline->bind( rec, 0 );
    for( unsigned int i = 0u; i != pushed.size(); ++i ) {
      if( props.query ) {
        const auto query_token = rec.begin(
          query_pool,
          i,
          vk::QueryControlFlags( 0 )
        );
        const auto r = range[ i ];
        const auto pc = push_constant_type()
          .set_matrix( matrix )
          .set_head( r.first );
        rec->pushConstants(
          **pipeline->get_layout(),
          pipeline->get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
          0u,
          sizeof( push_constant_type ),
          &pc
        );
        rec->draw( 1u, r.second, 0u, 0u );
      }
      else {
        const auto r = range[ i ];
        const auto pc = push_constant_type()
          .set_matrix( matrix )
          .set_head( r.first );
        rec->pushConstants(
          **pipeline->get_layout(),
          pipeline->get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
          0u,
          sizeof( push_constant_type ),
          &pc
        );
        rec->draw( 1u, r.second, 0u, 0u );
      }
    }
  }
  received = false;
}
std::uint32_t basic_occlusion_query::operator[]( occlusion_query_id_t i ) const {
  if( i >= pushed.size() ) return 0u;
  if( !received && props.query ) {
    result = query_pool->get_result< std::uint32_t >( 0u, pushed.size(), vk::QueryResultFlagBits::eWait );
    received = true;
  }
  return result[ i ];
}
void basic_occlusion_query::reset() {
  pushed.clear();
  if( props.query ) {
    query_pool->reset();
  }
}
const std::shared_ptr< image_view_t > &basic_occlusion_query::get_image_view() const {
  return output->get_image_view( 0u );
}

}

