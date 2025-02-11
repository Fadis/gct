#include <nlohmann/json.hpp>
#include <glm/vec4.hpp>
#include <gct/format.hpp>
#include <gct/shader_flag.hpp>
#include <gct/get_shader.hpp>
#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/get_device.hpp>
#include <gct/shader_flag.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compiled_aabb_scene_graph.hpp>
namespace gct::scene_graph {

void compiled_aabb_primitive::operator()(
  command_buffer_recorder_t &rec
) const {
  /*rec.bind_pipeline(
    pipeline
  );
  for( const auto &[bind_point,view]: prim->vertex_buffer ) {
    auto b = vertex->get( view.buffer );
    std::vector< vk::Buffer > raw_b{ **b };
    rec->bindVertexBuffers( bind_point, raw_b, view.offset );
  }*/
  rec.draw( 1, 1, 0, 0 );
}

void compiled_aabb_scene_graph::create_pipeline(
  const scene_graph &graph
) {
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
  auto dsci = pipeline_dynamic_state_create_info_t()
    .add_dynamic_state( vk::DynamicState::eViewport )
    .add_dynamic_state( vk::DynamicState::eScissor );
  auto cbsci = pipeline_color_blend_state_create_info_t();
  const auto &attachments = props.render_pass->get_props().get_attachment();
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
          .setFront( props.stencil_op )
          .setBack( props.stencil_op )
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
          .setFront( props.stencil_op )
          .setBack( props.stencil_op )
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
  const auto shader = load_shader( graph );
  auto gpci = graphics_pipeline_create_info_t()
      .add_stage( get_shader( shader, shader_flag_t::vertex ) )
      .add_stage( get_shader( shader, shader_flag_t::geometry ) )
      .add_stage( get_shader( shader, shader_flag_t::fragment ) )
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
          .set_basic(
            vk::PipelineViewportStateCreateInfo()
              .setViewportCount( 1 )
              .setScissorCount( 1 )
          )
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
      .set_layout( graph.get_resource()->pipeline_layout )
      .set_render_pass( props.render_pass, props.subpass )
      .rebuild_chain();
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
  if( enable_representive ) {
    gpci.set_representative_fragment_test(
      vk::PipelineRepresentativeFragmentTestStateCreateInfoNV()
        .setRepresentativeFragmentTestEnable( true )
    );
  }
#endif
  pipeline = graph.get_props().allocator_set.pipeline_cache->get_pipeline( gpci );
}
void compiled_aabb_scene_graph::create_vertex_buffer() {
  vertex_buffer_desc = resource->vertex->allocate(
    std::vector< glm::vec4 >{ { 0.f, 0.f, 0.f, 1.f } }
  );
}

compiled_aabb_scene_graph::compiled_aabb_scene_graph(
  const compiled_aabb_scene_graph_create_info &ci,
  const scene_graph &graph
) : props( ci ), resource( graph.get_resource() ) {
  const auto &device = get_device( *graph.get_props().allocator_set.allocator );
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
  enable_representive = device.get_activated_extensions().find( VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME ) != device.get_activated_extensions().end();
#else
  enable_conditional = false;
#endif
  create_vertex_buffer();
  create_pipeline( graph );
  load_graph( graph );
}
void compiled_aabb_scene_graph::load_graph(
  const scene_graph &graph
) {
  for( const auto &desc: resource->prim.get_descriptor() ) {
    const auto p = resource->prim.get( desc );
    auto p_ = load_primitive( p );
    prim.insert( std::make_pair( desc, p_ ) );
  }
}

void compiled_aabb_scene_graph::operator()( command_buffer_recorder_t &rec, std::uint32_t instance_count ) const {
  rec.bind_pipeline( pipeline );
  auto b = resource->vertex->get( vertex_buffer_desc );
  rec->bindVertexBuffers( 0u, { **b }, { 0u } );
  rec.draw( 1, instance_count, 0, 0 );
}

std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > compiled_aabb_scene_graph::load_shader(
  const scene_graph &graph
) {
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > shader;
  for( auto &path: std::filesystem::directory_iterator( props.shader ) ) {
    auto flag = get_shader_flag( path.path() );
    if( flag ) {
      shader.emplace(
        *flag,
        get_device( *graph.get_props().allocator_set.allocator ).get_shader_module(
          path.path().string()
        )
      );
    }
  }
  return shader;
}

compiled_aabb_primitive compiled_aabb_scene_graph::load_primitive(
  const std::shared_ptr< primitive > &p
) {
  return
    compiled_aabb_primitive()
      .set_prim( p );
}

void to_json( nlohmann::json &dest, const compiled_aabb_primitive &src ) {
  dest = nlohmann::json::object();
  if( src.prim ) {
    dest[ "prim" ] = *src.prim;
  }
}

}

