#include <iostream>
#include <boost/program_options.hpp>
#include <nlohmann/json.hpp>
#include <vulkan2json/ImageCreateInfo.hpp>
#include <gct/dds.hpp>
#include <gct/mmaped_file.hpp>
#include <gct/create_image_from_file.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/physical_device.hpp>
#include <gct/device_create_info.hpp>
#include <gct/device.hpp>
#include <gct/image.hpp>
#include <gct/image_create_info.hpp>
#include <gct/allocator.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/vulkanhpp.hpp>
#include <gct/io_context.hpp>
#include <vulkan2json/BufferImageCopy.hpp>

int main(
  int argc,
  const char *argv[]
) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "input,i", po::value< std::string >(), "DDS filename" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "input" ) ) {
    std::cout << desc << std::endl;
    exit( 0 );
  }
  gct::mmaped_file file( vm[ "input" ].as< std::string >() );
  const auto ci = gct::dds::get_dds_image_create_info( file.begin() );
  std::cout << nlohmann::json( ci ).dump( 2 ) << std::endl;
  auto iter = gct::dds::get_dds_data( ci, file.begin() );
  std::cout  << "file size : " << std::distance( file.begin(), file.end() ) << std::endl;
  for( ; iter != gct::dds::dds_iterator(); ++iter ) {
    std::cout  << "subimage : offset=" << std::distance( file.begin(), *iter ) << " size=" << iter.get_subimage_bytes() << std::endl;
  }

  using namespace gct;

  std::shared_ptr< instance_t > instance;
  std::shared_ptr< device_t > device;
  std::shared_ptr< queue_t > queue;
  std::shared_ptr< descriptor_pool_t > descriptor_pool;
  std::shared_ptr< pipeline_cache_t > pipeline_cache;
  std::shared_ptr< allocator_t > allocator;
  allocator_set_t allocator_set;


  std::vector< const char* > iext{};
  vulkanhpp::init();
  thread_pool::init();
  iext.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
  const auto ilayers = get_instance_layers();
  const auto iexts = get_instance_extensions( std::vector< const char* >() );
  auto ici = instance_create_info_t()
    .set_application_info(
      vk::ApplicationInfo()
        .setPApplicationName( "my_application" )
        .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
        .setApiVersion( VK_MAKE_VERSION( 1, 3, 0 ) )
    )
    .add_extension(
      iext.begin(), iext.end()
    );
  ici.add_layer(
    "VK_LAYER_KHRONOS_validation"
  );
#ifdef VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME
  ici.add_extension( VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME );
  ici.add_validation_feature_enable( vk::ValidationFeatureEnableEXT::eDebugPrintf );
#endif
  instance.reset( new instance_t( ici ) );
  instance->abort_on_validation_failure( false );
  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
      VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME
  } );

  std::vector< gct::queue_requirement_t > queue_requirements{
    gct::queue_requirement_t{
      vk::QueueFlagBits::eGraphics,
      0u,
      vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
      vk::QueueGlobalPriorityEXT(),
#endif
      {},
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer
    }
  };
  device = selected.create_device(
    queue_requirements,
    gct::device_create_info_t()
  );
  queue = device->get_queue( 0u );

  const auto dpci =
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags(
            vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet|
            vk::DescriptorPoolCreateFlagBits::eUpdateAfterBind
          )
          .setMaxSets( 65536 * 10 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 16 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 65536*5 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 65536*2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 65536*2 )
      .rebuild_chain();

  descriptor_pool = device->get_descriptor_pool( dpci );

  pipeline_cache = device->get_pipeline_cache();

  VmaAllocatorCreateInfo allocator_create_info{};
  allocator = device->get_allocator(
    allocator_create_info
  );

  auto state = gct::create_image_from_file(
    allocator,
    vm[ "input" ].as< std::string >(),
    vk::ImageUsageFlagBits::eSampled,
    true,
    gct::integer_attribute_t( 0 ),
    4u,
    true
  );

  std::cout << nlohmann::json( state.nonlinear_image->get_props() ).dump( 2 ) << std::endl;
  std::cout << nlohmann::json( state.copy_range ).dump( 2 ) << std::endl;
}


