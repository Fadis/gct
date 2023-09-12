#include <random>
#include <iostream>
#include <nlohmann/json.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/queue.hpp>
#include <gct/device.hpp>
#include <gct/allocator.hpp>
#include <gct/device_create_info.hpp>
#include <gct/image_create_info.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module_create_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/write_descriptor_set.hpp>
#include <gct/dnn/nnef.hpp>
#include <gct/dnn/linear.hpp>

struct spec_t {
  std::uint32_t input_length = 32u;
};

int main() {
  uint32_t iext_count = 0u;
  std::vector< const char* > iext{};
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        .set_application_info(
          vk::ApplicationInfo()
            .setPApplicationName( "my_application" )
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            .setApiVersion( VK_MAKE_VERSION( 1, 2, 0 ) )
        )
        .add_layer(
          "VK_LAYER_KHRONOS_validation"
        )
        .add_extension(
          iext.begin(), iext.end()
        )
    )
  );
  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME,
    VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME
  } );
 
  auto device = selected.create_device(
    std::vector< gct::queue_requirement_t >{
      gct::queue_requirement_t{
        vk::QueueFlagBits::eCompute,
        0u,
        vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
        vk::QueueGlobalPriorityEXT(),
#endif
        {},
        vk::CommandPoolCreateFlagBits::eResetCommandBuffer
      }
    },
    gct::device_create_info_t()
  );
  auto queue = device->get_queue( 0u );
  auto command_buffer = queue->get_command_pool()->allocate();

  auto allocator = device->get_allocator();
  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 10 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 5 )
      .rebuild_chain()
  );
  auto pipeline_cache = device->get_pipeline_cache();
  
  const unsigned int input_length = 2048u;
  const unsigned int output_length = 512u;
  
  std::mt19937 rng;
  std::uniform_real_distribution dist( -16.0, 16.0 );

  std::vector< float > host_input( input_length, float( 0 ) );
  for( unsigned int i = 0u; i != input_length; ++i ) {
    host_input[ i ] = float( dist( rng ) );
  }

  std::vector< float > host_weight( input_length * output_length, float( 0 ) );
  for( unsigned int i = 0u; i != output_length; ++i ) {
    for( unsigned int j = 0u; j != input_length; ++j ) {
      host_weight[ i * input_length + j ] = float( dist( rng ) );
    }
  }

  gct::nnef_data_t device_input;
  device_input.type.component = gct::numeric_component_type_t::float_;
  device_input.type.depth = 32u;
  device_input.type.rows = 1u;
  device_input.dim.push_back( input_length );
  
  gct::nnef_data_t device_weight;
  device_weight.type.component = gct::numeric_component_type_t::float_;
  device_weight.type.depth = 32u;
  device_weight.type.rows = 1u;
  device_weight.dim.push_back( output_length );
  device_weight.dim.push_back( input_length );
  
  {
    auto rec = command_buffer->begin();
    device_input.buffer = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_input.data() ),
      host_input.size() * sizeof( float ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst

    );
    device_weight.buffer = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_weight.data() ),
      host_weight.size() * sizeof( float ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  auto shaders = gct::dnn::load_shaders(
    device,
    CMAKE_CURRENT_BINARY_DIR "/shaders"
  );

  gct::dnn::operation::linear op(
    allocator,
    descriptor_pool,
    pipeline_cache,
    device_input,
    device_weight,
    shaders
  );

  std::vector< float > host_output( output_length, float( 0 ) );

  std::shared_ptr< std::vector< std::uint8_t > > host_output_byte;

  {
    auto rec = command_buffer->begin();
    op( rec );
    rec.compute_to_transfer_barrier(
      { op.get_output().buffer },
      {}
    );
    host_output_byte = rec.dump_buffer( allocator, op.get_output().buffer );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();
  std::copy(
    host_output_byte->begin(),
    host_output_byte->end(),
    reinterpret_cast< std::uint8_t* >( host_output.data() )
  );
  std::cout << "#!/usr/bin/env python" << std::endl;
  std::cout << "# -*- coding: utf-8 -*-" << std::endl;
  std::cout << "import numpy as np" << std::endl;

  std::cout << "a = np.array([" << std::endl;
  for( unsigned int i = 0u; i != input_length; ++i ) {
    std::cout << host_input[ i ];
    if( i != input_length - 1 ) std::cout << ", ";
  }
  std::cout << "])" << std::endl;
  std::cout << "b = np.matrix([" << std::endl;
  for( unsigned int i = 0u; i != output_length; ++i ) {
    std::cout << "  [ ";
    for( unsigned int j = 0u; j != input_length; ++j ) {
      std::cout << host_weight[ i * input_length + j ];
      if( j != input_length - 1 ) std::cout << ", ";
    }
    std::cout << " ]";
    if( i != output_length - 1 ) std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << "])" << std::endl;
  std::cout << "c = np.array([" << std::endl;
  for( unsigned int i = 0u; i != output_length; ++i ) {
    std::cout << host_output[ i ];
    if( i != output_length - 1 ) std::cout << ", ";
  }
  std::cout << "])" << std::endl;
  std::cout << "d = b@a" << std::endl;
  std::cout << "print( d - c )" << std::endl;
}

