#include <gct/gbuffer.hpp>
#include <gct/allocator.hpp>
#include <gct/compute.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/hierarchical_raster_occlusion_culling.hpp>
#include <gct/key_value.hpp>
#include <gct/format.hpp>
#include <gct/onesweep.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <vulkan/vulkan_enums.hpp>

namespace gct::scene_graph {

hierarchical_raster_occlusion_culling::hierarchical_raster_occlusion_culling(
  const hierarchical_raster_occlusion_culling_create_info &ci,
  const instance_list &il
) : props( ci ), resource( il.get_resource() ) {
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
      .set_external_depth( props.external_depth )
  );

  sort_input = props.allocator_set.allocator->create_mappable_buffer(
    il.get_draw_list().size() * sizeof( key_value_t ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );
  
  sort_output = props.allocator_set.allocator->create_mappable_buffer(
    il.get_draw_list().size() * sizeof( key_value_t ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );

  node_offset.push_back(
    index_range()
      .set_count( il.get_draw_list().size() )
      .set_offset( 0u )
  );
  leaf_count = il.get_draw_list().size();
  node_count = il.get_draw_list().size();
  while( node_offset.back().count != 1u ) {
    node_offset.push_back(
      index_range()
        .set_count( node_offset.back().count / 2u + ( ( node_offset.back().count % 2u ) ? 1u : 0u ) )
        .set_offset( node_offset.back().offset + node_offset.back().count )
    );
    node_count += node_offset.back().count;
  }

  bvh = props.allocator_set.allocator->create_mappable_buffer(
    node_count * sizeof( node_type ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );

  visibility = props.allocator_set.allocator->create_mappable_buffer(
    node_count * sizeof( std::uint32_t ),
    vk::BufferUsageFlagBits::eStorageBuffer|
    vk::BufferUsageFlagBits::eTransferDst
  );

  setup_sort = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.setup_sort_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { "instance_resource_index", resource->instance_resource_index->get_buffer() } )
      .add_resource( { "visibility_pool", resource->last_visibility->get_buffer() } )
      .add_resource( { "aabb_pool", resource->aabb->get_buffer() } )
      .add_resource( { "resource_pair", resource->resource_pair->get_buffer() } )
      .add_resource( { "sort_input", sort_input } )
  );

  sort = std::make_shared< gct::onesweep >(
    gct::onesweep_create_info( props.sort )
      .set_allocator_set( props.allocator_set )
      .set_input( sort_input->get_buffer() )
      .set_output( sort_output->get_buffer() )
  );

  setup_leaf = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.setup_leaf_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { "instance_resource_index", resource->instance_resource_index->get_buffer() } )
      .add_resource( { "visibility_pool", resource->last_visibility->get_buffer() } )
      .add_resource( { "aabb_pool", resource->aabb->get_buffer() } )
      .add_resource( { "resource_pair", resource->resource_pair->get_buffer() } )
      .add_resource( { "sort_output", sort_output } )
      .add_resource( { "bvh", bvh } )
  );
  
  setup_node = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.setup_node_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { "bvh", bvh } )
  );
  
  mark_leaf = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.mark_leaf_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { "instance_resource_index", resource->instance_resource_index->get_buffer() } )
      .add_resource( { "visibility_pool", resource->last_visibility->get_buffer() } )
      .add_resource( { "aabb_pool", resource->aabb->get_buffer() } )
      .add_resource( { "resource_pair", resource->resource_pair->get_buffer() } )
      .add_resource( { "sort_output", sort_output } )
      .add_resource( { "bvh", bvh } )
      .add_resource( { "visibility", visibility } )
  );
  
  mark_node = std::make_shared< gct::compute >(
    gct::compute_create_info()
      .set_allocator_set( props.allocator_set )
      .set_shader( props.mark_node_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { "bvh", bvh } )
      .add_resource( { "visibility", visibility } )
  );
  
  global_uniform = props.allocator_set.allocator->create_mappable_buffer(
    sizeof( global_uniform_type ),
    vk::BufferUsageFlagBits::eStorageBuffer
  );
  
  vertex_buffer = props.allocator_set.allocator->create_mappable_buffer(
    sizeof( glm::vec4 ), vk::BufferUsageFlagBits::eVertexBuffer
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

  evaluate = std::make_shared< graphics >(
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
      .add_resource( { "instance_resource_index", resource->instance_resource_index->get_buffer() } )
      .add_resource( { "visibility_pool", resource->last_visibility->get_buffer() } )
      .add_resource( { "aabb_pool", resource->aabb->get_buffer() } )
      .add_resource( { "resource_pair", resource->resource_pair->get_buffer() } )
      .add_resource( { "bvh", bvh } )
      .add_resource( { "visibility", visibility } )
      .add_resource( { "global_uniforms", global_uniform } )
  );
}
void hierarchical_raster_occlusion_culling::setup(
  command_buffer_recorder_t &rec
) {
  {
    const auto pc = push_constant_type()
      .set_src_count( leaf_count )
      .set_src_offset( 0u )
      .set_dest_count( leaf_count )
      .set_dest_offset( 0u );
    rec->pushConstants(
      **setup_sort->get_pipeline()->get_props().get_layout(),
      setup_sort->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
      0u,
      sizeof( push_constant_type ),
      &pc
    );
  }
  (*setup_sort)( rec, 0u, leaf_count, 1u, 1u );
  rec.barrier( { sort_input->get_buffer() }, {} );
  (*sort)( rec, leaf_count );
  rec.barrier( { sort_output->get_buffer() }, {} );
  rec.fill( bvh->get_buffer(), 0xFFFFFFFF );
  rec.barrier( { bvh->get_buffer() }, {} );
  {
    const auto pc = push_constant_type()
      .set_src_count( leaf_count )
      .set_src_offset( 0u )
      .set_dest_count( leaf_count )
      .set_dest_offset( 0u );
    rec->pushConstants(
      **setup_leaf->get_pipeline()->get_props().get_layout(),
      setup_leaf->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
      0u,
      sizeof( push_constant_type ),
      &pc
    );
  }
  (*setup_leaf)( rec, 0u, leaf_count, 1u, 1u );
  rec.barrier( { bvh->get_buffer() }, {} );
  for( unsigned int i = 1u; i != node_offset.size(); ++i ) {
    {
      const auto pc = push_constant_type()
        .set_src_count( node_offset[ i - 1u ].count )
        .set_src_offset( node_offset[ i - 1u ].offset )
        .set_dest_count( node_offset[ i ].count )
        .set_dest_offset( node_offset[ i ].offset );
      rec->pushConstants(
        **setup_node->get_pipeline()->get_props().get_layout(),
        setup_node->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        0u,
        sizeof( push_constant_type ),
        &pc
      );
    }
    (*setup_node)( rec, 0u, node_offset[ i ].count, 1u, 1u );
    rec.barrier( { bvh->get_buffer() }, {} );
  }
}
void hierarchical_raster_occlusion_culling::operator()(
  command_buffer_recorder_t &rec,
  const glm::mat4 &projection,
  const glm::mat4 &camera,
  const glm::vec4 &eye_pos
) {
  {
    const auto global_data = global_uniform_type()
      .set_projection( projection )
      .set_camera( camera )
      .set_eye_pos( eye_pos );
    rec.copy( global_data, global_uniform );
    rec.transfer_to_graphics_barrier( { global_uniform->get_buffer() }, {} );
  }
  rec.fill( visibility, 0u );
  rec.barrier( { visibility->get_buffer() }, {} );
  {
    const auto pc = push_constant_type()
      .set_src_count( leaf_count )
      .set_src_offset( 0u )
      .set_dest_count( leaf_count )
      .set_dest_offset( 0u );
    rec->pushConstants(
      **mark_leaf->get_pipeline()->get_props().get_layout(),
      mark_leaf->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
      0u,
      sizeof( push_constant_type ),
      &pc
    );
  }
  (*mark_leaf)( rec, 0u, leaf_count, 1u, 1u );
  rec.barrier( { visibility->get_buffer() }, {} );
  for( unsigned int i = 1u; i != node_offset.size(); ++i ) {
    {
      const auto pc = push_constant_type()
        .set_src_count( node_offset[ i - 1u ].count )
        .set_src_offset( node_offset[ i - 1u ].offset )
        .set_dest_count( node_offset[ i ].count )
        .set_dest_offset( node_offset[ i ].offset );
      rec->pushConstants(
        **mark_node->get_pipeline()->get_props().get_layout(),
        mark_node->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        0u,
        sizeof( push_constant_type ),
        &pc
      );
    }
    (*mark_node)( rec, 0u, node_offset[ i ].count, 1u, 1u );
    rec.barrier( { visibility->get_buffer() }, {} );
  }
  {
    auto render_pass_token = rec.begin_render_pass(
      output->get_render_pass_begin_info( 0 ),
      vk::SubpassContents::eInline
    );
    rec->bindVertexBuffers( 0u, { **vertex_buffer }, { 0u } );
    evaluate->bind( rec, 0 );
    rec->draw( 1u, node_count, 0u, 0u );
  }
  rec.barrier( { resource->last_visibility->get_buffer() }, {} );
}


}

