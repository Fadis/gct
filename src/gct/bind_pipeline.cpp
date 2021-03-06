

#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/ray_tracing_pipeline.hpp>

namespace gct {
  void command_buffer_recorder_t::bind_pipeline(
    vk::PipelineBindPoint bind_point,
    std::shared_ptr< compute_pipeline_t > pipeline
  ) {
    (*get_factory())->bindPipeline(
      bind_point,
      **pipeline
    );
    get_factory()->unbound()->keep.push_back( pipeline );
  }
  void command_buffer_recorder_t::bind_pipeline(
    vk::PipelineBindPoint bind_point,
    std::shared_ptr< ray_tracing_pipeline_t > pipeline
  ) {
    (*get_factory())->bindPipeline(
      bind_point,
      **pipeline
    );
    get_factory()->unbound()->keep.push_back( pipeline );
  }
}

