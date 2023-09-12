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

struct spec_t {
  std::int32_t filter_size_x = 0;
  std::int32_t filter_size_y = 0;
  std::int32_t filter_size_z = 0;
  std::int32_t lpadding = 0;
  std::int32_t rpadding = 0;
  std::int32_t tpadding = 0;
  std::int32_t bpadding = 0;
  std::int32_t stride_x = 0;
  std::int32_t stride_y = 0;
  std::int32_t dilation_x = 0;
  std::int32_t dilation_y = 0;
  std::int32_t input_dim_x = 0;
  std::int32_t input_dim_y = 0;
  std::int32_t input_dim_z = 0;
  std::int32_t output_dim_x = 0;
  std::int32_t output_dim_y = 0;
  std::int32_t output_dim_z = 0;
  float border_value = 0.0f;
  std::int32_t bias_mode = 0;
  float bias_value = 0.0f;
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
    CMAKE_CURRENT_BINARY_DIR "/convolution_f32_f32.comp.spv"
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
    3, 3, 3,
    1, 1, 1, 1,
    1, 1,
    1, 1,
    16, 16, 3,
    16, 16, 2,
    0.2f,
    1,
    0.0f
  };

  auto pipeline = pipeline_cache->get_pipeline(
    gct::compute_pipeline_create_info_t()
      .set_stage(
        gct::pipeline_shader_stage_create_info_t()
          .set_shader_module( shader )
          .set_specialization_info(
            gct::specialization_info_t< spec_t >()
              .set_data( spec_t( spec ) )
              .add_map< std::uint32_t >( 1, offsetof( spec_t, filter_size_x ) )
              .add_map< std::uint32_t >( 2, offsetof( spec_t, filter_size_y ) )
              .add_map< std::uint32_t >( 3, offsetof( spec_t, filter_size_z ) )
              .add_map< std::uint32_t >( 4, offsetof( spec_t, lpadding ) )
              .add_map< std::uint32_t >( 5, offsetof( spec_t, rpadding ) )
              .add_map< std::uint32_t >( 6, offsetof( spec_t, tpadding ) )
              .add_map< std::uint32_t >( 7, offsetof( spec_t, bpadding ) )
              .add_map< std::uint32_t >( 8, offsetof( spec_t, stride_x ) )
              .add_map< std::uint32_t >( 9, offsetof( spec_t, stride_y ) )
              .add_map< std::uint32_t >( 10, offsetof( spec_t, dilation_x ) )
              .add_map< std::uint32_t >( 11, offsetof( spec_t, dilation_y ) )
              .add_map< std::uint32_t >( 12, offsetof( spec_t, input_dim_x ) )
              .add_map< std::uint32_t >( 13, offsetof( spec_t, input_dim_y ) )
              .add_map< std::uint32_t >( 14, offsetof( spec_t, input_dim_z ) )
              .add_map< std::uint32_t >( 15, offsetof( spec_t, output_dim_x ) )
              .add_map< std::uint32_t >( 16, offsetof( spec_t, output_dim_y ) )
              .add_map< std::uint32_t >( 17, offsetof( spec_t, output_dim_z ) )
              .add_map< float >( 18, offsetof( spec_t, border_value ) )
              .add_map< float >( 19, offsetof( spec_t, bias_mode ) )
              .add_map< float >( 20, offsetof( spec_t, bias_value ) )
          )
      )
      .set_layout( pipeline_layout )
  );
  nlohmann::json pipeline_json = *pipeline;
  auto allocator = device->get_allocator();

  std::mt19937 rng;
  std::uniform_real_distribution dist( -16.0, 16.0 );

  std::vector< float > host_input( spec.input_dim_x * spec.input_dim_y * spec.input_dim_z, float( 0 ) );
  std::fill( host_input.begin(), host_input.end(), 0.f );
  host_input[ 25 ] = 1.0f;
  std::vector< float > host_output( spec.output_dim_x * spec.output_dim_y * spec.output_dim_z, float( 0 ) );

  const unsigned int dispatch_x = ( spec.output_dim_x / 32 + ( ( spec.output_dim_x % 32 ) ? 1 : 0 ) ) * 32;
  const unsigned int dispatch_y = ( spec.output_dim_y / 32 + ( ( spec.output_dim_y % 32 ) ? 1 : 0 ) ) * 32;
  const unsigned int dispatch_z = spec.output_dim_z;
  std::cout << "dispatch " << dispatch_x << " " << dispatch_y << " " << dispatch_z << std::endl;

  std::shared_ptr< gct::buffer_t > device_input;
  gct::nnef_data_t device_weight;
  gct::nnef_data_t device_bias;
  std::shared_ptr< gct::buffer_t > device_output;
  {
    auto rec = command_buffer->begin();
    device_input = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_input.data() ),
      host_input.size() * sizeof( float ),
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst

    );
    device_weight = rec.load_nnef_data(
      allocator,
      "/home/fadis/conv_only/conv.dat",
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst
    );
    device_bias = rec.load_nnef_data(
      allocator,
      "/home/fadis/conv_only/bias.dat",
      vk::BufferUsageFlagBits::eStorageBuffer|
      vk::BufferUsageFlagBits::eTransferDst
    );
    device_output = rec.load_buffer(
      allocator,
      reinterpret_cast< void* >( host_output.data() ),
      host_output.size() * sizeof( float ),
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
        .set_basic( (*descriptor_set)[ "input_vector" ] )
        .add_buffer( device_input ),
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "weight" ] )
        .add_buffer( device_weight.buffer ),
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "bias" ] )
        .add_buffer( device_bias.buffer ),
      gct::write_descriptor_set_t()
        .set_basic( (*descriptor_set)[ "output_vector" ] )
        .add_buffer( device_output )
    }
  );

  std::vector< std::uint8_t > host_output_byte;

  {
    auto rec = command_buffer->begin();
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eCompute,
      pipeline_layout,
      descriptor_set
    );
    rec.bind_pipeline( pipeline );
    rec.dispatch_threads( dispatch_x, dispatch_y, dispatch_z );
    rec.barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eComputeShader,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      { device_output },
      {}
    );
    rec.dump_buffer( allocator, device_output ).then(
      [&]( std::vector< std::uint8_t > &&v ) {
        host_output_byte = std::move( v );
      }
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();
  std::copy(
    host_output_byte.begin(),
    host_output_byte.end(),
    reinterpret_cast< std::uint8_t* >( host_output.data() )
  );
  for( unsigned int k = 0u; k != spec.output_dim_z; ++k ) {
    for( unsigned int j = 0u; j != spec.output_dim_y; ++j ) {
      for( unsigned int i = 0u; i != spec.output_dim_x; ++i ) {
        std::cout << host_output[ i + j * spec.output_dim_x + k * spec.output_dim_x * spec.output_dim_y ] << " ";
      }
      std::cout << std::endl;
    }
    std::cout << std::endl;
  }
}

