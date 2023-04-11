

#include <gct/image.hpp>
#include <gct/allocator.hpp>
#include <gct/device.hpp>
#include <gct/command_pool.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/format.hpp>
#include <gct/descriptor_set.hpp>
#include <gct/pipeline_layout.hpp>

namespace gct {
  void command_buffer_recorder_t::bind_descriptor_set(
    vk::PipelineBindPoint bind_point,
    std::shared_ptr< pipeline_layout_t > pipeline_layout,
    std::shared_ptr< descriptor_set_t > descriptor_set
  ) {
    (*get_factory())->bindDescriptorSets(
      bind_point,
      **pipeline_layout,
      0,
      **descriptor_set,
      {}
    );
    get_factory()->unbound()->keep.push_back( pipeline_layout );
    get_factory()->unbound()->keep.push_back( descriptor_set );
  }
  void command_buffer_recorder_t::bind_descriptor_set(
    vk::PipelineBindPoint bind_point,
    std::uint32_t offset,
    std::shared_ptr< pipeline_layout_t > pipeline_layout,
    std::shared_ptr< descriptor_set_t > descriptor_set
  ) {
    (*get_factory())->bindDescriptorSets(
      bind_point,
      **pipeline_layout,
      offset,
      **descriptor_set,
      {}
    );
    get_factory()->unbound()->keep.push_back( pipeline_layout );
    get_factory()->unbound()->keep.push_back( descriptor_set );
  }
  void command_buffer_recorder_t::bind_descriptor_set(
    vk::PipelineBindPoint bind_point,
    std::uint32_t offset,
    std::shared_ptr< pipeline_layout_t > pipeline_layout,
    const std::vector< std::shared_ptr< descriptor_set_t > > &descriptor_set
  ) {
    std::vector< vk::DescriptorSet > temp;
    temp.reserve( descriptor_set.size() );
    std::transform(
      descriptor_set.begin(),
      descriptor_set.end(),
      std::back_inserter( temp ),
      []( const auto &v ) { return **v; }
    );
    (*get_factory())->bindDescriptorSets(
      bind_point,
      **pipeline_layout,
      offset,
      temp,
      {}
    );
    get_factory()->unbound()->keep.push_back( pipeline_layout );
    get_factory()->unbound()->keep.push_back( descriptor_set );
  }
}

