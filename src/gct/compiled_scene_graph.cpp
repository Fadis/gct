#include <nlohmann/json.hpp>
#include "gct/exception.hpp"
#include <gct/format.hpp>
#include <gct/shader_flag.hpp>
#include <gct/get_shader.hpp>
#include <gct/allocator.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/buffer.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/get_device.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/compiled_scene_graph.hpp>
namespace gct::scene_graph {

void compiled_primitive::operator()(
  command_buffer_recorder_t &rec
) const {
  rec.bind_pipeline(
    pipeline
  );
  for( const auto &[bind_point,view]: prim->vertex_buffer ) {
    auto b = vertex->get( view.buffer );
    std::vector< vk::Buffer > raw_b{ **b };
    rec->bindVertexBuffers( bind_point, raw_b, view.offset );
  }
  if( !prim->indexed ) {
    rec.draw( prim->count, 1, 0, 0 );
  }
  else {
    auto b = vertex->get( prim->index_buffer.buffer );
    rec->bindIndexBuffer( **b, prim->index_buffer.offset, prim->index_buffer_type );
    rec.draw_indexed( prim->count, 1, 0, 0, 0 );
  }
}

compiled_scene_graph::compiled_scene_graph(
  const compiled_scene_graph_create_info &ci,
  const scene_graph &graph
) : props( ci ), resource( graph.get_resource() ) {
  load_graph( graph );
}
void compiled_scene_graph::load_graph(
  const scene_graph &graph
) {
  auto s = load_shader( graph );
  for( const auto &desc: resource->prim.get_descriptor() ) {
    const auto p = resource->prim.get( desc );
    auto p_ = load_primitive( graph, p, s );
    prim.insert( std::make_pair( desc, p_ ) );
  }
}

std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > compiled_scene_graph::load_shader(
  const scene_graph &graph
) {
  std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > shader;
  for( auto &path: std::filesystem::directory_iterator( props.shader ) ) {
    auto flag = get_shader_flag( path.path() );
    if( flag ) {
      shader.emplace(
        *flag,
        get_device( *graph.get_props().allocator ).get_shader_module(
          path.path().string()
        )
      );
    }
  }
  return shader;
}

compiled_primitive compiled_scene_graph::load_primitive(
  const scene_graph &graph,
  const std::shared_ptr< primitive > &p,
  const std::unordered_map< shader_flag_t, std::shared_ptr< shader_module_t > > &shader
) {
  auto ci = p->pipeline_create_info; 
  auto cbsci = pipeline_color_blend_state_create_info_t();
  const auto &attachments = props.render_pass->get_props().get_attachment();
  for( const auto &attachment: attachments ) {
    if( format_to_aspect( attachment.format ) == vk::ImageAspectFlagBits::eColor ) {
      cbsci
        .add_attachment(
          vk::PipelineColorBlendAttachmentState()
            .setBlendEnable( p->blend )
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
          .setDepthWriteEnable( VK_TRUE )
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
  auto dsci = ci.get_dynamic();
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
  ci
    .set_depth_stencil( std::move( dssci ) )
    .set_color_blend( std::move( cbsci ) )
    .set_dynamic( std::move( dsci ) )
    .set_render_pass( props.render_pass, props.subpass )
    .rebuild_chain();

  ci.add_stage( get_shader( shader, p->vs_flag ) );
  ci.add_stage( get_shader( shader, p->gs_flag ) );
  ci.add_stage( get_shader( shader, int( props.shader_mask ) ? props.shader_mask : p->fs_flag ) );
  ci.rebuild_chain();
  return
    compiled_primitive()
      .set_vertex(
        resource->vertex
      )
      .set_prim( p )
      //.set_matrix( n->get_matrix() )
      //.set_aabb(
      //  resource->aabb->allocate( p->descriptor.aabb, n->get_matrix() )
      //)
      .set_visibility(
        resource->visibility->allocate()
      )
      .set_pipeline(
        graph.get_props().pipeline_cache->get_pipeline( ci )
      );
}

void to_json( nlohmann::json &dest, const compiled_primitive &src ) {
  dest = nlohmann::json::object();
  if( src.vertex ) {
    dest[ "vertex" ] = *src.vertex;
  }
  if( src.prim ) {
    dest[ "prim" ] = *src.prim;
  }
  if( src.visibility ) {
    dest[ "visibility" ] = *src.visibility;
  }
  if( src.pipeline ) {
    dest[ "visibility" ] = *src.pipeline;
  }
}

}

