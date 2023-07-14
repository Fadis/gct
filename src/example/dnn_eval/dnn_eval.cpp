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
#include <gct/dnn/load_image.hpp>
#include <gct/dnn/synset_mapping.hpp>

struct spec_t {
  std::uint32_t input_length = 32u;
};

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "model,m", po::value< std::string >(), "NNEF filename" )
    ( "image,i", po::value< std::string >(), "image filename" )
    ( "label,l", po::value< std::string >(), "label filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "model" ) || !vm.count( "image" ) || !vm.count( "label" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const std::string nnef_filename = vm[ "model" ].as< std::string >();
  const std::string image_filename = vm[ "image" ].as< std::string >();
  const std::string label_filename = vm[ "label" ].as< std::string >();

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

  const auto label = gct::dnn::load_synset_mapping( label_filename );

  std::shared_ptr< gct::dnn::graph > graph;
  gct::nnef_data_t device_input;
  gct::nnef_data_t device_output;

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
    device_input = graph->get_input();
    device_output = graph->get_output();
    gct::dnn::load_image(
      allocator,
      image_filename,
      device_input,
      rec
    );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  std::shared_ptr< std::vector< std::uint8_t > > host_output_byte;
  {
    auto rec = command_buffer->begin();
    (*graph)( rec );
    rec.compute_to_transfer_barrier(
      { device_output.buffer }, {}
    );
    host_output_byte = rec.dump_buffer( allocator, device_output.buffer );
  }
  command_buffer->execute(
    gct::submit_info_t()
  );
  command_buffer->wait_for_executed();

  std::vector< float > result( host_output_byte->size() / sizeof( float ) );
  std::copy(
    host_output_byte->begin(),
    host_output_byte->end(),
    reinterpret_cast< std::uint8_t* >( result.data() )
  );
  std::vector< std::tuple< unsigned int, std::string, float > > sorted;
  for( unsigned int i = 0u; i != result.size(); ++i ) {
    sorted.push_back( std::make_tuple( i, label[ i ], result[ i ] ) );
  }
  std::sort( sorted.begin(), sorted.end(), []( const auto &l, const auto &r ) { return std::get< 2 >( l ) < std::get< 2 >( r ); } );
  for( unsigned int i = 0u; i != 10u; ++i ) {
    std::cout << std::get< 0 >( sorted[ sorted.size() - 1u - i ] ) << " " << std::get< 1 >( sorted[ sorted.size() - 1u - i ] ) << " " << std::get< 2 >( sorted[ sorted.size() - 1u - i ] ) << std::endl;
  }
}

