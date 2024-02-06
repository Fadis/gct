#include "gct/exception.hpp"
#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/compute_pipeline.hpp>
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
#include <gct/ray_tracing_pipeline.hpp>
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
#include <gct/shader.hpp>
#endif

namespace gct {
  void command_buffer_recorder_t::bind(
    std::shared_ptr< compute_pipeline_t > pipeline,
    const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
  ) {
    bind_descriptor_set(
      vk::PipelineBindPoint::eCompute,
      0u,
      pipeline->get_props().get_layout(),
      descriptor_set
    );
    bind_pipeline(
      pipeline
    );
    get_factory()->set_current_compute_pipeline( pipeline );
  }
  void command_buffer_recorder_t::bind(
    std::shared_ptr< graphics_pipeline_t > pipeline,
    const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
  ) {
    bind_descriptor_set(
      vk::PipelineBindPoint::eGraphics,
      0u,
      pipeline->get_props().get_layout(),
      descriptor_set
    );
    bind_pipeline(
      pipeline
    );
    get_factory()->set_current_graphics_pipeline( pipeline );
  }
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  void command_buffer_recorder_t::bind(
    std::shared_ptr< ray_tracing_pipeline_t > pipeline,
    const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
  ) {
    bind_descriptor_set(
      vk::PipelineBindPoint::eRayTracingKHR,
      0u,
      pipeline->get_props().get_layout(),
      descriptor_set
    );
    bind_pipeline(
      pipeline
    );
  }
#endif
#ifdef VK_EXT_SHADER_OBJECT_EXTENSION_NAME
  void command_buffer_recorder_t::bind(
    const std::vector< std::shared_ptr< shader_t > > &shader
  ) {
    std::vector< vk::ShaderStageFlagBits > stage;
    stage.reserve( shader.size() );
    std::transform(
      shader.begin(),
      shader.end(),
      std::back_inserter( stage ),
      []( const auto &s ) {
        return s->get_stage();
      }
    );
    std::vector< vk::ShaderEXT > raw_shader;
    raw_shader.reserve( shader.size() );
    std::transform(
      shader.begin(),
      shader.end(),
      std::back_inserter( raw_shader ),
      []( const auto &s ) {
        return **s;
      }
    );
    (*get_factory())->bindShadersEXT(
      stage, raw_shader  
    );
    get_factory()->unbound()->keep.push_back( shader );
  }
  void command_buffer_recorder_t::bind(
    const std::vector< std::shared_ptr< shader_t > > &shader,
    const std::shared_ptr< pipeline_layout_t > &pipeline_layout,
    const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
  ) {
    if(  shader.empty() ) {
      throw exception::invalid_argument( "command_buffer_recorder_t::bind : at least one shader is required.", __FILE__, __LINE__ );
    }
    if( shader[ 0 ]->is_compute() ) {
      bind_descriptor_set(
        vk::PipelineBindPoint::eCompute,
        0u,
        pipeline_layout,
        descriptor_set
      );
    }
    else if( shader[ 0 ]->is_graphics() ) {
      bind_descriptor_set(
        vk::PipelineBindPoint::eGraphics,
        0u,
        pipeline_layout,
        descriptor_set
      );
    }
    else if( shader[ 0 ]->is_ray_trace() ) {
      bind_descriptor_set(
        vk::PipelineBindPoint::eRayTracingKHR,
        0u,
        pipeline_layout,
        descriptor_set
      );
    }
    else {
      throw exception::invalid_argument( "command_buffer_recorder_t::bind : unknown pipeline bind point.", __FILE__, __LINE__ );
    }
    bind( shader );
  }
#endif
}

