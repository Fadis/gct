#include <fstream>
#include <iterator>
#include <iostream>
#include <gct/pipeline_layout.hpp>
#include <gct/render_pass.hpp>
#include <gct/graphics_pipeline_create_info.hpp>

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
    chained = false;
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
  graphics_pipeline_create_info_t &graphics_pipeline_create_info_t::clear_dynamic() {
    dynamic.reset();
    chained = false;
    return *this;
  }
}

