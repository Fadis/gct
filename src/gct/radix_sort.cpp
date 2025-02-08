#include <cstdint>
#include <nlohmann/json.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/compute.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/command_buffer_recorder.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv_member_pointer.hpp>
#include <gct/key_value.hpp>
#include <gct/radix_sort.hpp>


namespace gct {

radix_sort::radix_sort(
  const radix_sort_create_info &ci
) : property_type( ci ) {
  max_size = std::min(
    props.input->get_props().get_basic().size / sizeof( key_value_t ), 
    props.output->get_props().get_basic().size / sizeof( key_value_t )
  );
  local_offset =
    props.allocator->create_buffer(
      sizeof( std::uint32_t ) * max_size,
      vk::BufferUsageFlagBits::eStorageBuffer,
      VMA_MEMORY_USAGE_GPU_ONLY
    );
  workgroup_offset =
    props.allocator->create_buffer(
      sizeof( std::uint32_t ) * 1024u,
      vk::BufferUsageFlagBits::eStorageBuffer,
      VMA_MEMORY_USAGE_GPU_ONLY
    );
  local_sum.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.local_sum_shader )
      .set_swapchain_image_count( 2u )
      .set_resources( props.resources )
      .add_resource( { props.input_name, { props.input, props.output } } )
      .add_resource( { props.local_offset_name, local_offset } )
      .add_resource( { props.workgroup_offset_name, workgroup_offset } )
      .add_resource( { props.output_name, { props.output, props.input } } )
  ) );

  global_sum.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.global_sum_shader )
      .set_swapchain_image_count( 1u )
      .set_resources( props.resources )
      .add_resource( { props.workgroup_offset_name, workgroup_offset } )
  ) );

  sort.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.sort_shader )
      .set_swapchain_image_count( 2u )
      .set_resources( props.resources )
      .add_resource( { props.input_name, { props.input, props.output } } )
      .add_resource( { props.local_offset_name, local_offset } )
      .add_resource( { props.workgroup_offset_name, workgroup_offset } )
      .add_resource( { props.output_name, { props.output, props.input } } )
  ) );

  small_sort.reset( new gct::compute(
    gct::compute_create_info()
      .set_allocator( props.allocator )
      .set_descriptor_pool( props.descriptor_pool )
      .set_pipeline_cache( props.pipeline_cache )
      .set_shader( props.small_sort_shader )
      .set_swapchain_image_count( 1u )
      .add_resource( { props.input_name, props.input } )
      .add_resource( { props.output_name, props.output } )
  ) );
}
void radix_sort::operator()(
  command_buffer_recorder_t &rec,
  std::uint32_t size
) const {
  if( size > max_size ) {
    throw -1;
  }
  const auto pc_mp = local_sum->get_reflection().get_push_constant_member_pointer( "PushConstants" );
  std::vector< std::uint8_t > pc( pc_mp.get_aligned_size() );
  pc.data()->*pc_mp[ "n" ] = size;
  if( size <= 1024u ) {
    rec->pushConstants(
      **small_sort->get_pipeline()->get_props().get_layout(),
      small_sort->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
      pc_mp.get_offset(),
      pc_mp.get_aligned_size(),
      pc.data()
    );
    (*small_sort)( rec, 0u, size, 1u, 1u );
    rec.barrier( { props.output }, {} );
  }
  else {
    for( std::uint32_t i = 0u; i != 31u; ++i ) {
      pc.data()->*pc_mp[ "digit" ] = i;
      rec->pushConstants(
        **local_sum->get_pipeline()->get_props().get_layout(),
        local_sum->get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        pc_mp.get_offset(),
        pc_mp.get_aligned_size(),
        pc.data()
      );
      (*local_sum)( rec, i & 0x1u, size, 1u, 1u );
      rec.barrier( { workgroup_offset }, {} );
      (*global_sum)( rec, 0u, 1024u, 1u, 1u );
      rec.barrier( { workgroup_offset, local_offset }, {} );
      (*sort)( rec, i & 0x1u, size, 1u, 1u );
      rec.barrier( { props.input, props.output, local_offset }, {} );
    }
  }
}

}

