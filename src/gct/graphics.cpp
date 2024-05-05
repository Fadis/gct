#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/named_resource.hpp>
#include <gct/get_device.hpp>
#include <gct/device.hpp>
#include <gct/spv2vk.hpp>
#include <gct/graphics.hpp>
#include <gct/graphics_create_info.hpp>
#include <gct/shader_module_reflection.hpp>

namespace gct {
  graphics::graphics(
    const graphics_create_info &ci
  ) :
    props( ci ) {
    auto &device = get_device( *props.pipeline_cache );
    for( const auto &s: props.shaders ) {
      props.pipeline_create_info.add_stage( device.get_shader_module( s ) );
    }
    for( const auto &s: props.shader_module ) {
      if( s ) {
        props.pipeline_create_info.add_stage( s );
      }
    }
    props.pipeline_create_info.rebuild_chain();
    if( props.external_pipeline_layout ) {
      pipeline_layout = props.external_pipeline_layout;
    }
    else {
      gct::descriptor_set_layout_create_info_t dslci;
      for( const auto &s: props.pipeline_create_info.get_stage() ) {
        dslci.add_binding( s.get_shader_module()->get_props().get_reflection() );
      }
      descriptor_set_layout = device.get_descriptor_set_layout( dslci );
     
      gct::pipeline_layout_create_info_t plci;
      plci.add_descriptor_set_layout( descriptor_set_layout );
      for( const auto &s: props.pipeline_create_info.get_stage() ) {
        plci.add_push_constant_range( s.get_shader_module() );
      }
      pipeline_layout = device.get_pipeline_layout( plci );
    }

    const auto vs = std::find_if(
      props.pipeline_create_info.get_stage().begin(),
      props.pipeline_create_info.get_stage().end(),
      []( const auto &s ) -> bool {
        return spv2vk(
          s.get_shader_module()->get_props().get_reflection()->shader_stage
        ) == vk::ShaderStageFlagBits::eVertex;
      }
    );
    if( vs == props.pipeline_create_info.get_stage().end() ) {
      throw -1;
    }
    std::tie(vistat,vamap,stride) = gct::get_vertex_attributes(
      device,
      vs->get_shader_module()->get_props().get_reflection()
    );
 
    if( props.external_descriptor_set ) {
      std::vector< write_descriptor_set_t > temp;
      for( const auto &r: props.resources ) {
        temp.push_back( { r, 0 } );
      }
      props.external_descriptor_set->update( temp );
    }
    else {
      for( unsigned int i = 0u; i != props.swapchain_image_count; ++i ) {
        descriptor_set.push_back( props.descriptor_pool->allocate(
          descriptor_set_layout
        ) );
        std::vector< write_descriptor_set_t > temp;
        for( const auto &r: props.resources ) {
          temp.push_back( { r, i } );
        }
        descriptor_set.back()->update( temp );
      }
    }
    if( props.pipeline_create_info.get_color_blend().get_attachment().empty() ) {
      props.pipeline_create_info
        .set_color_blend(
          pipeline_color_blend_state_create_info_t( props.pipeline_create_info.get_color_blend() )
            .add_attachment()
        );
    }
    pipeline = props.pipeline_cache->get_pipeline(
      props.pipeline_create_info
        .set_vertex_input( vistat )
        .set_layout( pipeline_layout )
        .fill_untouched()
    );
  }
  void graphics::bind(
    command_buffer_recorder_t &rec,
    unsigned int image_index
  ) const {
    if( props.external_descriptor_set ) {
      rec.bind(
        pipeline,
        { props.external_descriptor_set }
      );
    }
    else {
      rec.bind(
        pipeline,
        { descriptor_set[ image_index ] }
      );
    }
  }
}

