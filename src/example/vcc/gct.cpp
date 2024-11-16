#include <cstdint>
#include <random>
#include <iostream>
#include <chrono>
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
#include <gct/key_value.hpp>
#include <gct/radix_sort.hpp>

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
    false,
    true
  );

  const auto input_data =
    res.allocator->create_mappable_buffer(
      sizeof( float ) * 260,
      vk::BufferUsageFlagBits::eStorageBuffer
    );
  
  const auto output_data =
    res.allocator->create_mappable_buffer(
      sizeof( float ) * 260,
      vk::BufferUsageFlagBits::eStorageBuffer
    );

  gct::compute comp(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/test.comp.spv" )
  );
  {
    auto mapped = input_data->map< float >();
    for( unsigned int i = 0u; i != 260; ++i ) {
      mapped[ i ] = i;
    }
  }
  const auto input_data_addr = input_data->get_buffer()->get_address(); 
  const auto output_data_addr = output_data->get_buffer()->get_address(); 
  auto command_buffer = res.queue->get_command_pool()->allocate();
  const auto begin = std::chrono::high_resolution_clock::now();
  {
    {
      auto rec = command_buffer->begin();
      rec.sync_to_device( input_data );
      rec.transfer_to_compute_barrier( { input_data->get_buffer() }, {} );
      std::array< vk::DeviceAddress, 2u > addrs{
        *output_data_addr,
        *input_data_addr
      };
      rec->pushConstants(
        **comp.get_pipeline()->get_props().get_layout(),
        comp.get_pipeline()->get_props().get_layout()->get_props().get_push_constant_range()[ 0 ].stageFlags,
        0u,
        sizeof( vk::DeviceAddress ) * addrs.size(),
        addrs.data()
      );
      comp( rec, 0, 256, 1, 1 );
      rec.sync_to_host( output_data );
    }
    command_buffer->execute_and_wait();
  }
  const auto end = std::chrono::high_resolution_clock::now();
  //std::cout << "elapsed time : " << std::chrono::duration_cast< std::chrono::microseconds >( end - begin ).count() << "us" << std::endl;
  std::vector< float > host_output_data;
  {
    auto mapped = output_data->map< float >();
    std::copy( mapped.begin(), mapped.end(), std::back_inserter( host_output_data ) );
  }
  for( unsigned int i = 0u; i != 260; ++i ) {
    std::cout << host_output_data[ i ] << std::endl;
  }
}

