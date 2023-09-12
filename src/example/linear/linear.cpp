#include <random>
#include <iostream>
#include <nlohmann/json.hpp>
#include <boost/program_options.hpp>
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
#include <gct/dnn/graph.hpp>

struct spec_t {
  std::uint32_t input_length = 32u;
};

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "NNEF filename" )
    ( "noexec,n", po::bool_switch(), "don't exec on host" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const std::string nnef_filename = vm[ "input" ].as< std::string >();
  const bool noexec = vm[ "noexec" ].as< bool >();

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
    VK_NV_COOPERATIVE_MATRIX_EXTENSION_NAME,
    VK_EXT_MEMORY_BUDGET_EXTENSION_NAME
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
          .setMaxSets( 100 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 100 )
      .rebuild_chain()
  );
  auto pipeline_cache = device->get_pipeline_cache();

  nnef::Graph host_graph;
  std::string error;
  if( !nnef::load_graph( nnef_filename, host_graph, error, "" ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( !nnef::infer_shapes( host_graph, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( !nnef::allocate_buffers( host_graph, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( host_graph.inputs.empty() ) {
    throw -1;
  }
  std::mt19937 rng;
  std::uniform_real_distribution dist( -16.0, 16.0 );
  auto input_tensor = host_graph.tensors.find( host_graph.inputs[ 0 ] );
  if( input_tensor == host_graph.tensors.end() ) {
    throw -1;
  }
  for( 
    auto iter = reinterpret_cast< float* >( input_tensor->second.data.data() );
    iter != std::next(
      reinterpret_cast< float* >( input_tensor->second.data.data() ),
      input_tensor->second.data.size() /  sizeof( float )
    );
    ++iter
  ) {
    *iter = float( dist( rng ) );
  }

  auto output_tensor = host_graph.tensors.find( host_graph.outputs[ 0 ] );
  if( output_tensor == host_graph.tensors.end() ) {
    throw -1;
  }
 
  if( !noexec ) {
    if( !nnef::execute( host_graph, error ) ) {
      std::cerr << error << std::endl;
      throw -1;
    }
  }

  std::shared_ptr< gct::dnn::graph > graph;
  gct::nnef_data_t device_input;
  gct::nnef_data_t device_output;

  std::cout << "input size : " << input_tensor->second.data.size() / sizeof( float ) << std::endl;
  std::cout << "output size : " << output_tensor->second.data.size() / sizeof( float ) << std::endl;

  {
    auto rec = command_buffer->begin();
    graph = std::make_shared< gct::dnn::graph >(
      device,
      allocator,
      descriptor_pool,
      pipeline_cache,
      nnef_filename,
      CMAKE_CURRENT_BINARY_DIR "/shaders",
      rec
    );
    std::cout << nlohmann::json( graph->get_input() ) << std::endl;
    std::cout << nlohmann::json( graph->get_output() ) << std::endl;
    device_input = graph->get_input();
    device_output = graph->get_output();
    rec.load_buffer(
      allocator,
      reinterpret_cast< const void* >( input_tensor->second.data.data() ),
      input_tensor->second.data.size(),
      device_input.buffer
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  std::vector< std::uint8_t > host_output_byte;
  {
    auto rec = command_buffer->begin();
    (*graph)( rec );
    rec.compute_to_transfer_barrier(
      { device_output.buffer }, {}
    );
    rec.dump_buffer( allocator, device_output.buffer ).then(
      [&]( std::vector< std::uint8_t > &&v ) {
        host_output_byte = std::move( v );
      }
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  std::vector< float > expected( output_tensor->second.data.size() / sizeof( float ) );
  std::copy(
    output_tensor->second.data.begin(),
    output_tensor->second.data.end(),
    reinterpret_cast< std::uint8_t* >( expected.data() )
  );
  std::vector< float > result( host_output_byte.size() / sizeof( float ) );
  std::copy(
    host_output_byte.begin(),
    host_output_byte.end(),
    reinterpret_cast< std::uint8_t* >( result.data() )
  );
  for( unsigned int i = 0u; i != expected.size(); ++i ) {
    const auto diff = std::abs( expected[ i ] - result[ i ] );
    if( diff > 0.00001f ) {
      std::cout << "[ " << i << " ] " << expected[ i ] << " != " << result[ i ] << std::endl;
    }
  }
}

