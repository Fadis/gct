#include <fstream>
#include <iterator>
#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/device.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/render_pass.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/get_device.hpp>
#include <gct/gbuffer.hpp>
#include <vulkan2json/GraphicsPipelineCreateInfo.hpp>
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
#include <vulkan2json/AttachmentSampleCountInfoAMD.hpp>
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
#include <vulkan2json/GraphicsPipelineShaderGroupsCreateInfoNV.hpp>
#endif
#if defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) && defined(VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME)
#include <vulkan2json/MultiviewPerViewAttributesInfoNVX.hpp>
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
#include <vulkan2json/PipelineCreationFeedbackCreateInfoEXT.hpp>
#endif
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
#include <vulkan2json/PipelineCompilerControlCreateInfoAMD.hpp>
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
#include <vulkan2json/PipelineDiscardRectangleStateCreateInfoEXT.hpp>
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
#include <vulkan2json/PipelineFragmentShadingRateEnumStateCreateInfoNV.hpp>
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
#include <vulkan2json/PipelineFragmentShadingRateStateCreateInfoKHR.hpp>
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
#include <vulkan2json/PipelineRenderingCreateInfoKHR.hpp>
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
#include <vulkan2json/PipelineRepresentativeFragmentTestStateCreateInfoNV.hpp>
#endif 

namespace gct {
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::rebuild_chain() {
    if( chained ) return *this;
    for( auto &s: stage ) s.rebuild_chain();
    raw_stage.clear();
    raw_stage.reserve( stage.size() );
    std::transform(
      stage.begin(),
      stage.end(),
      std::back_inserter( raw_stage ),
      []( const auto &v ) {
        return v.get_basic();
      }
    );
    if( !raw_stage.empty() )
      basic
        .setStageCount( raw_stage.size() )
        .setPStages( raw_stage.data() );
    else
      basic
        .setStageCount( 0u )
        .setPStages( nullptr );

    if( vertex_input ) {
      vertex_input->rebuild_chain();
      basic
        .setPVertexInputState( &vertex_input->get_basic() );
    }
    else
      basic
        .setPVertexInputState( nullptr );

    if( input_assembly ) {
      input_assembly->rebuild_chain();
      basic
        .setPInputAssemblyState( &input_assembly->get_basic() );
    }
    else
      basic
        .setPInputAssemblyState( nullptr );

    if( tessellation ) {
      tessellation->rebuild_chain();
      basic
        .setPTessellationState( &tessellation->get_basic() );
    }
    else
      basic
        .setPTessellationState( nullptr );

    if( viewport ) {
      viewport->rebuild_chain();
      basic
        .setPViewportState( &viewport->get_basic() );
    }
    else
      basic
        .setPViewportState( nullptr );

    if( rasterization ) {
      rasterization->rebuild_chain();
      basic
        .setPRasterizationState( &rasterization->get_basic() );
    }
    else
      basic
        .setPRasterizationState( nullptr );

   if( multisample ) {
      multisample->rebuild_chain();
      basic
        .setPMultisampleState( &multisample->get_basic() );
    }
    else
      basic
        .setPMultisampleState( nullptr );

    if( depth_stencil ) {
      depth_stencil->rebuild_chain();
      basic
        .setPDepthStencilState( &depth_stencil->get_basic() );
    }
    else {
      basic
        .setPDepthStencilState( nullptr );
    }

    if( color_blend ) {
      color_blend->rebuild_chain();
      basic
        .setPColorBlendState( &color_blend->get_basic() );
    }
    else
      basic
        .setPColorBlendState( nullptr );

    if( dynamic ) {
      dynamic->rebuild_chain();
      basic
        .setPDynamicState( &dynamic->get_basic() );
    }
    else
      basic
        .setPDynamicState( nullptr );

    if( layout )
      basic.setLayout( **layout );
    else
      basic.setLayout( nullptr );

    if( render_pass )
      basic
        .setRenderPass( **render_pass )
        .setSubpass( subpass );
    else
      basic
        .setRenderPass( nullptr )
        .setSubpass( 0u );

    LIBGCT_EXTENSION_BEGIN_REBUILD_CHAIN
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( attachment_sample_count ) 
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( shader_group ) 
#endif
#if defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) && defined(VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME)
    LIBGCT_EXTENSION_REBUILD_CHAIN( multiview_per_view_attributes ) 
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( creation_feedback )
#endif
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( compiler_control ) 
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( discard_rectangle ) 
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate_nv ) 
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( fragment_shading_rate ) 
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( rendering ) 
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_REBUILD_CHAIN( representative_fragment_test ) 
#endif 
    LIBGCT_EXTENSION_END_REBUILD_CHAIN
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::add_stage( const pipeline_shader_stage_create_info_t &v ) {
    stage.push_back( v );
    stage.back().rebuild_chain();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::add_stage( const std::shared_ptr< shader_module_t > &v ) {
    if( v ) {
      if( !v->get_props().has_reflection() )
        throw exception::invalid_argument( "Reflection is required to set shader directly to pipeline", __FILE__, __LINE__ );
      add_stage(
        pipeline_shader_stage_create_info_t()
          .set_shader_module( v )
      );
    }
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::add_stage( const std::vector< std::shared_ptr< shader_module_t > > &v ) {
    for( const auto &e: v ) {
      add_stage( e );
    }
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_stage() {
    stage.clear();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_layout( const std::shared_ptr< pipeline_layout_t > &v ) {
    layout = v;
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_layout( const std::shared_ptr< descriptor_set_layout_t > &v ) {
    return set_layout(
      get_device( *v ).get_pipeline_layout(
        gct::pipeline_layout_create_info_t()
          .add_descriptor_set_layout(
            v
          )
      )
    );
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_layout() {
    layout.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_render_pass( const std::shared_ptr< render_pass_t > &v, std::uint32_t sub ) {
    render_pass = v;
    subpass = sub;
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_render_pass() {
    render_pass.reset();
    subpass = 0u;
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_vertex_input( const pipeline_vertex_input_state_create_info_t &v ) {
    vertex_input.reset( new pipeline_vertex_input_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_vertex_input() {
    vertex_input.reset( new pipeline_vertex_input_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_vertex_input() {
    vertex_input.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_input_assembly( const pipeline_input_assembly_state_create_info_t &v ) {
    input_assembly.reset( new pipeline_input_assembly_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_input_assembly() {
    input_assembly.reset( new pipeline_input_assembly_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_input_assembly() {
    input_assembly.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_tessellation( const pipeline_tessellation_state_create_info_t &v ) {
    tessellation.reset( new pipeline_tessellation_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_tessellation() {
    tessellation.reset( new pipeline_tessellation_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_tessellation() {
    tessellation.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_viewport( const pipeline_viewport_state_create_info_t &v ) {
    viewport.reset( new pipeline_viewport_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_viewport() {
    viewport.reset( new pipeline_viewport_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_viewport() {
    viewport.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_rasterization( const pipeline_rasterization_state_create_info_t &v ) {
    rasterization.reset( new pipeline_rasterization_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_rasterization() {
    rasterization.reset( new pipeline_rasterization_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_rasterization() {
    rasterization.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_multisample( const pipeline_multisample_state_create_info_t &v ) {
    multisample.reset( new pipeline_multisample_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_multisample() {
    multisample.reset( new pipeline_multisample_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_multisample() {
    multisample.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_depth_stencil( const pipeline_depth_stencil_state_create_info_t &v ) {
    depth_stencil.reset( new pipeline_depth_stencil_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_depth_stencil() {
    depth_stencil.reset( new pipeline_depth_stencil_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_depth_stencil() {
    depth_stencil.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_color_blend( const pipeline_color_blend_state_create_info_t &v ) {
    color_blend.reset( new pipeline_color_blend_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_color_blend() {
    color_blend.reset( new pipeline_color_blend_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_color_blend() {
    color_blend.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_dynamic( const pipeline_dynamic_state_create_info_t &v ) {
    dynamic.reset( new pipeline_dynamic_state_create_info_t( v ) );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_dynamic() {
    dynamic.reset( new pipeline_dynamic_state_create_info_t() );
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_dynamic() {
    dynamic.reset();
    chained = false;
    return *this;
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::fill_untouched() {
    if( !vertex_input ) set_vertex_input();
    if( !input_assembly ) set_input_assembly();
    if( !tessellation ) set_tessellation();
    if( !viewport ) set_viewport();
    if( !rasterization ) set_rasterization();
    if( !multisample ) set_multisample();
    if( !depth_stencil ) set_depth_stencil();
    if( !color_blend ) set_color_blend();
    if( !dynamic ) set_dynamic();
    return *this;
  }
  const pipeline_viewport_state_create_info_t &graphics_pipeline_create_info_t::get_viewport() const {
    if( viewport ) {
      return *viewport;
    }
    else {
      static const pipeline_viewport_state_create_info_t dummy;
      return dummy;
    }
  }
  const pipeline_color_blend_state_create_info_t &graphics_pipeline_create_info_t::get_color_blend() const {
    if( color_blend ) {
      return *color_blend;
    }
    else {
      static const pipeline_color_blend_state_create_info_t dummy;
      return dummy;
    }
  }
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::set_gbuffer(
    const gbuffer &g
  ) {
    set_viewport(
      gct::pipeline_viewport_state_create_info_t()
        .add_size( g.get_props().width, g.get_props().height )
    );
    set_render_pass( g.get_render_pass(), 0 );
    pipeline_color_blend_state_create_info_t temp;
    for( unsigned int i = 0u; i != g.get_props().layer; ++i ) {
      temp.add_attachment();
    }
    set_color_blend( temp );
    return *this;
  }
  void to_json( nlohmann::json &root, const graphics_pipeline_create_info_t &v ) {
    root = nlohmann::json::object();
    root[ "basic" ] = v.get_basic();
    root[ "basic" ][ "pStages" ] = v.stage;
    if( v.vertex_input )
      root[ "basic" ][ "pVertexInputState" ] = *v.vertex_input;
    else
      root[ "basic" ][ "pVertexInputState" ] = nullptr;
    if( v.input_assembly )
      root[ "basic" ][ "pInputAssemblyState" ] = *v.input_assembly;
    else
      root[ "basic" ][ "pInputAssemblyState" ] = nullptr;
    if( v.tessellation )
      root[ "basic" ][ "pTessellationState" ] = *v.tessellation;
    else
      root[ "basic" ][ "pTessellationState" ] = nullptr;
    if( v.viewport )
      root[ "basic" ][ "pViewportState" ] = *v.viewport;
    else
      root[ "basic" ][ "pViewportState" ] = nullptr;
    if( v.rasterization )
      root[ "basic" ][ "pRasterizationState" ] = *v.rasterization;
    else
      root[ "basic" ][ "pRasterizationState" ] = nullptr;
    if( v.multisample )
      root[ "basic" ][ "pMultisampleState" ] = *v.multisample;
    else
      root[ "basic" ][ "pMultisampleState" ] = nullptr;
    if( v.depth_stencil )
      root[ "basic" ][ "pDepthStencilState" ] = *v.depth_stencil;
    else
      root[ "basic" ][ "pDepthStencilState" ] = nullptr;
    if( v.color_blend )
      root[ "basic" ][ "pColorBlendState" ] = *v.color_blend;
    else
      root[ "basic" ][ "pColorBlendState" ] = nullptr;
    if( v.dynamic )
      root[ "basic" ][ "pDynamicState" ] = *v.dynamic;
    else
      root[ "basic" ][ "pDynamicState" ] = nullptr;
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( attachment_sample_count ) 
#endif
#ifdef VK_NV_DEVICE_GENERATED_COMMANDS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( shader_group ) 
#endif
#if defined(VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME) && defined(VK_NVX_MULTIVIEW_PER_VIEW_ATTRIBUTES_EXTENSION_NAME)
    LIBGCT_EXTENSION_TO_JSON( multiview_per_view_attributes ) 
#endif
#ifdef VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( creation_feedback )
#endif
#ifdef VK_AMD_PIPELINE_COMPILER_CONTROL_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( compiler_control ) 
#endif
#ifdef VK_EXT_DISCARD_RECTANGLES_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( discard_rectangle ) 
#endif
#ifdef VK_NV_FRAGMENT_SHADING_RATE_ENUMS_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate_nv ) 
#endif
#ifdef VK_KHR_FRAGMENT_SHADING_RATE_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( fragment_shading_rate ) 
#endif
#ifdef VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( rendering ) 
#endif
#ifdef VK_NV_REPRESENTATIVE_FRAGMENT_TEST_EXTENSION_NAME
    LIBGCT_EXTENSION_TO_JSON( representative_fragment_test ) 
#endif 
  }
}

