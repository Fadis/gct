#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/shader_module.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/ray_tracing_pipeline.hpp>

namespace gct {
  void command_buffer_recorder_t::bind_pipeline(
    std::shared_ptr< compute_pipeline_t > pipeline
  ) {
    (*get_factory())->bindPipeline(
      vk::PipelineBindPoint::eCompute,
      **pipeline
    );
    local_size_is_available = false;
    const auto dim = pipeline->get_props().get_dim();
    if(
      dim.x != 0 &&
      dim.y != 0 &&
      dim.z != 0
    ) {
      local_size_is_available = true;
      local_size[ 0 ] = dim.x;
      local_size[ 1 ] = dim.y;
      local_size[ 2 ] = dim.z;
    }
    get_factory()->unbound()->keep.push_back( pipeline );
  }
  void command_buffer_recorder_t::bind_pipeline(
    std::shared_ptr< graphics_pipeline_t > pipeline
  ) {
    (*get_factory())->bindPipeline(
      vk::PipelineBindPoint::eGraphics,
      **pipeline
    );
    local_size_is_available = false;
    get_factory()->unbound()->keep.push_back( pipeline );
  }
#ifdef VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME
  void command_buffer_recorder_t::bind_pipeline(
    std::shared_ptr< ray_tracing_pipeline_t > pipeline
  ) {
    (*get_factory())->bindPipeline(
      vk::PipelineBindPoint::eRayTracingKHR,
      **pipeline
    );
    local_size_is_available = false;
    get_factory()->unbound()->keep.push_back( pipeline );
  }
#endif
}

