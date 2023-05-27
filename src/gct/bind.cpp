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
}

