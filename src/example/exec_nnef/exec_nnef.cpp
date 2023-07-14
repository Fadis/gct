#include <iostream>
#include <boost/program_options.hpp>
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

#include <gct/dnn/nnef2json.hpp>

struct spec_t {
  std::uint32_t local_x_size = 0u;
  std::uint32_t local_y_size = 0u;
  float value = 0.f;
};

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "NNEF filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const std::string nnef_filename = vm[ "input" ].as< std::string >();

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
  
  auto allocator = device->get_allocator();

  nnef::Graph graph;
  std::string error;
  if( !nnef::load_graph( nnef_filename, graph, error, "" ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( !nnef::infer_shapes( graph, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }
  if( !nnef::allocate_buffers( graph, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }

  if( !graph.inputs.empty() ) {
    auto input_tensor = graph.tensors.find( graph.inputs[ 0 ] );
    if( input_tensor != graph.tensors.end() ) {
      std::fill( input_tensor->second.data.begin(), input_tensor->second.data.begin(), 0 );
      reinterpret_cast< float* >( input_tensor->second.data.data() )[ 0 ] = 1.0;
    }
    else {
      throw -1;
    }
  }
  else {
    throw -1;
  }

  if( !nnef::execute( graph, error ) ) {
    std::cerr << error << std::endl;
    throw -1;
  }

  {
    auto weight = graph.tensors.find( "variable" );
    if( weight != graph.tensors.end() ) {
      std::cout << "size " << weight->second.data.size() << std::endl;
      std::cout << "dimmension ";
      for( const auto &s: weight->second.shape ) {
        std::cout << s << " ";
      }
      std::cout << std::endl;
      for( unsigned int l = 0u; l != weight->second.shape[ 0 ]; ++l ) {
        for( unsigned int k = 0u; k != weight->second.shape[ 1 ]; ++k ) {
          for( unsigned int j = 0u; j != weight->second.shape[ 2 ]; ++j ) {
            for( unsigned int i = 0u; i != weight->second.shape[ 3 ]; ++i ) {
              std::cout << reinterpret_cast< float* >( weight->second.data.data() )[ i + j * 3u + k * 3u * 3u + l * 3u * 3u * 3u ] << " ";
            }
            std::cout << std::endl;
          }
          std::cout << std::endl;
        }
        std::cout << std::endl;
      }
    }
  }
  {
    auto bias = graph.tensors.find( "variable_1" );
    if( bias != graph.tensors.end() ) {
      std::cout << "size " << bias->second.data.size() << std::endl;
      std::cout << "dimmension ";
      for( const auto &s: bias->second.shape ) {
        std::cout << s << " ";
      }
      std::cout << std::endl;
      for( unsigned int j = 0u; j != bias->second.shape[ 0 ]; ++j ) {
        for( unsigned int i = 0u; i != bias->second.shape[ 1 ]; ++i ) {
          std::cout << reinterpret_cast< float* >( bias->second.data.data() )[ i + j * 1u ] << " ";
        }
        std::cout << std::endl;
      }
      std::cout << std::endl;
    }
  }

  if( !graph.outputs.empty() ) {
    auto output_tensor = graph.tensors.find( graph.outputs[ 0 ] );
    if( output_tensor != graph.tensors.end() ) {
      std::cout << "size " << output_tensor->second.data.size() << std::endl;
      std::cout << "dimmension ";
      for( const auto &s: output_tensor->second.shape ) {
        std::cout << s << " ";
      }
      std::cout << std::endl;
      for( unsigned int k = 0u; k != output_tensor->second.shape[ 1 ]; ++k ) {
        for( unsigned int j = 0u; j != output_tensor->second.shape[ 2 ]; ++j ) {
          for( unsigned int i = 0u; i != output_tensor->second.shape[ 3 ]; ++i ) {
            std::cout << reinterpret_cast< float* >( output_tensor->second.data.data() )[ i + j * 16u + k * 16u * 16u ] << " ";
          }
          std::cout << std::endl;
        }
        std::cout << std::endl;
      }
    }
    else {
      throw -1;
    }
  }
  else {
    throw -1;
  }
}

