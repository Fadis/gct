#include <cstdint>
#include <random>
#include <iostream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/shader_module_reflection.hpp>
#include <gct/spv_member_pointer.hpp>

struct key_value_t {
  std::uint32_t key = 0u;
  std::uint32_t value = 0u;
};

int main( int argc, const char *argv[] ) {
  const gct::common_sample_setup res(
    argc, argv,
    {
      VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
      VK_KHR_MAINTENANCE1_EXTENSION_NAME,
      VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME,
      VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME
    },
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet|
            vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind
          )
          .setMaxSets( 64 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 64 )
      .rebuild_chain(),
    false
  );

  const std::uint32_t size = 2048u;

  const auto input_data =
    res.allocator->create_mappable_buffer(
      sizeof( key_value_t ) * size,
      vk::BufferUsageFlagBits::eStorageBuffer
    );

  const auto workgroup_offset =
    res.allocator->create_mappable_buffer(
      sizeof( std::uint32_t ) * size / 1024u,
      vk::BufferUsageFlagBits::eStorageBuffer
    );
  
  const auto output_data =
    res.allocator->create_mappable_buffer(
      sizeof( std::uint32_t ) * size,
      vk::BufferUsageFlagBits::eStorageBuffer
    );

  const auto prefix_sum = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/radix_sort32/prefix_sum.comp.spv" )
      .set_swapchain_image_count( 1u )
      .add_resource( { "input_data", input_data } )
      .add_resource( { "workgroup_offset", workgroup_offset } )
      .add_resource( { "output_data", output_data } )
  );

  std::uniform_int_distribution< std::uint32_t > dist( 0, 0x80000000 );
  std::mt19937 engine;

  std::vector< key_value_t > host_input_data;
  for( unsigned int i = 0u; i != size; ++i ) {
    host_input_data.push_back({
      dist( engine ),
      dist( engine )
    });
  }
  {
    auto mapped = input_data->map< key_value_t >();
    std::copy( host_input_data.begin(), host_input_data.end(), mapped.begin() );
  }
  auto command_buffer = res.queue->get_command_pool()->allocate();
  const auto &pc_mp = prefix_sum.get_reflection().get_push_constant_member_pointer( "PushConstants" );
  std::vector< std::uint8_t > pc( pc_mp.get_aligned_size() );
  pc.data()->*pc_mp[ "n" ] = size;
  pc.data()->*pc_mp[ "digit" ] = 0u;
  {
    {
      auto rec = command_buffer->begin();
      rec.sync_to_device( input_data );
      rec.transfer_to_compute_barrier( { input_data->get_buffer() }, {} );
      rec->pushConstants(
        **prefix_sum.get_pipeline()->get_props().get_layout(),
        prefix_sum.get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        pc_mp.get_offset(),
        pc_mp.get_aligned_size(),
        pc.data()
      );
      prefix_sum( rec, 0u, size, 1u, 1u );
      rec.transfer_to_compute_barrier( { output_data->get_buffer() }, {} );
      rec.sync_to_host( output_data );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::uint32_t > host_output_data;
  {
    auto mapped = output_data->map< std::uint32_t >();
    std::copy( mapped.begin(), mapped.end(), std::back_inserter( host_output_data ) );
  }
  std::vector< key_value_t > dest( size );
  std::uint32_t sep = host_output_data[ size - 1u ];
  for( unsigned int i = 0u; i != size; ++i ) {
    std::uint32_t di = i == 0u ? 0u : host_output_data[ i - 1u ];
    if( host_input_data[ i ].key & 0x1 ) {
      dest[ i - di + sep ] = host_input_data[ i ];
    }
    else {
      dest[ di ] = host_input_data[ i ];
    }
  }
  for( unsigned int i = 0u; i != size; ++i ) {
    std::cout << dest[ i ].key << "\t" << dest[ i ].value << std::endl;
  }
}

