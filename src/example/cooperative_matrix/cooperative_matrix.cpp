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
#include <gct/gltf.hpp>
#include <gct/half.hpp>

struct spec_t {
  std::uint32_t m = 0u;
  std::uint32_t n = 0u;
  std::uint32_t k = 0u;
};

int main() {
  using namespace half_float::literal;
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
  
  auto shader = device->get_shader_module(
    CMAKE_CURRENT_BINARY_DIR "/shader.comp.spv"
  );
  
  auto descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        shader->get_props().get_reflection()
      )
      .rebuild_chain()
  );

  auto descriptor_set = descriptor_pool->allocate( descriptor_set_layout );

  auto pipeline_layout = device->get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
      .add_push_constant_range(
        vk::PushConstantRange()
          .setStageFlags( vk::ShaderStageFlagBits::eCompute )
          .setOffset( 0 )
          .setSize( 32u )
      )
  );

  auto pipeline_cache = device->get_pipeline_cache();

  spec_t spec{
    64u,
    64u,
    64u
  };

  auto pipeline = pipeline_cache->get_pipeline(
    gct::compute_pipeline_create_info_t()
      .set_stage(
        gct::pipeline_shader_stage_create_info_t()
          .set_shader_module( shader )
          .set_specialization_info(
            gct::specialization_info_t< spec_t >()
              .set_data( spec_t( spec ) )
              .add_map< std::uint32_t >( 1, offsetof( spec_t, m ) )
              .add_map< std::uint32_t >( 2, offsetof( spec_t, n ) )
              .add_map< std::uint32_t >( 3, offsetof( spec_t, k ) )
          )
      )
      .set_layout( pipeline_layout )
  );
  nlohmann::json pipeline_json = *pipeline;
  auto allocator = device->get_allocator();

  std::mt19937 rng;
  std::uniform_real_distribution dist( -16.0, 16.0 );

  std::vector< half_float::half > host_a( spec.m * spec.k, half_float::half( 0 ) );
  for( unsigned int i = 0u; i != spec.k; ++i ) {
    for( unsigned int j = 0u; j != spec.m; ++j ) {
      host_a[ i * spec.m + j ] = half_float::half( dist( rng ) );
    }
  }
  std::vector< half_float::half > host_b( spec.k * spec.n, half_float::half( 0 ) );
  for( unsigned int i = 0u; i != spec.n; ++i ) {
    for( unsigned int j = 0u; j != spec.k; ++j ) {
      host_b[ i * spec.k + j ] = half_float::half( dist( rng ) );
    }
  }
  std::vector< half_float::half > host_d( spec.m * spec.n, half_float::half( 0 ) );

  std::shared_ptr< gct::buffer_t > a;
  std::shared_ptr< gct::buffer_t > b;
  std::shared_ptr< gct::buffer_t > d;
  {
    auto rec = command_buffer->begin();
    a = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_a.data() ),
      host_a.size() * sizeof( half_float::half ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst

    );
    b = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_b.data() ),
      host_b.size() * sizeof( half_float::half ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst
    );
    d = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_d.data() ),
      host_d.size() * sizeof( half_float::half ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst|
      vk::BufferUsageFlagBits::eTransferSrc
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  descriptor_set->update(
    {
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "a" ] )
        .add_buffer( a ),
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "b" ] )
        .add_buffer( b ),
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "d" ] )
        .add_buffer( d )
    }
  );

  std::vector< std::uint8_t > host_d_dest;

  {
    auto rec = command_buffer->begin();
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eCompute,
      pipeline_layout,
      descriptor_set
    );
    rec.bind_pipeline( pipeline );
    rec.dispatch_threads( spec.m/8, spec.n/8, 32 );
    rec.barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      { d },
      {}
    );
    rec.dump_buffer( allocator, d ).then(
      [&]( std::vector< std::uint8_t > &&v ) {
        host_d_dest = std::move( v );
      }
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();
  std::vector< half_float::half > host_d_float( host_d_dest.size() / sizeof( half_float::half ) );
  std::copy(
    host_d_dest.begin(),
    host_d_dest.end(),
    reinterpret_cast< std::uint8_t* >( host_d_float.data() )
  );
  std::cout << "#!/usr/bin/env python" << std::endl;
  std::cout << "# -*- coding: utf-8 -*-" << std::endl;
  std::cout << "import numpy as np" << std::endl;

  std::cout << "a = np.matrix([" << std::endl;
  for( unsigned int i = 0u; i != spec.m; ++i ) {
    std::cout << "  [ ";
    for( unsigned int j = 0u; j != spec.k; ++j ) {
      std::cout << host_a[ i * spec.k + j ];
      if( j != spec.k - 1 ) std::cout << ", ";
    }
    std::cout << " ]";
    if( i != spec.m - 1 ) std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << "])" << std::endl;
  std::cout << "b = np.matrix([" << std::endl;
  for( unsigned int i = 0u; i != spec.k; ++i ) {
    std::cout << "  [ ";
    for( unsigned int j = 0u; j != spec.n; ++j ) {
      std::cout << host_b[ i * spec.n + j ];
      if( j != spec.n - 1 ) std::cout << ", ";
    }
    std::cout << " ]";
    if( i != spec.k - 1 ) std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << "])" << std::endl;
  std::cout << "c = np.matrix([" << std::endl;
  for( unsigned int i = 0u; i != spec.n; ++i ) {
    std::cout << "  [ ";
    for( unsigned int j = 0u; j != spec.m; ++j ) {
      std::cout << host_d_float[ i * spec.m + j ];
      if( j != spec.m - 1 ) std::cout << ", ";
    }
    std::cout << " ]";
    if( i != spec.n - 1 ) std::cout << ",";
    std::cout << std::endl;
  }
  std::cout << "])" << std::endl;
  std::cout << "d = a * b;" << std::endl;
  std::cout << "print( d - c )" << std::endl;
}

