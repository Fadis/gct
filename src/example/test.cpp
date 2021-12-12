#include <iostream>
#include <unordered_set>
#include <nlohmann/json.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gct/get_extensions.hpp>
#include <gct/instance.hpp>
#include <gct/glfw.hpp>
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
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/acceleration_structure.hpp>
#include <gct/acceleration_structure_geometry.hpp>
#include <gct/acceleration_structure_build_geometry_info.hpp>
#include <gct/acceleration_structure_geometry_triangles_data.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::image_t > color;
  std::shared_ptr< gct::framebuffer_t > framebuffer;
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  bool initial = true;
};

int main() {
  gct::glfw::get();
  uint32_t iext_count = 0u;
  auto exts = glfwGetRequiredInstanceExtensions( &iext_count );
  std::vector< const char* > iext{};
  for( uint32_t i = 0u; i != iext_count; ++i )
    iext.push_back( exts[ i ] );
  const auto ilayers = gct::get_instance_layers();
  const auto iexts = gct::get_instance_extensions( std::vector< const char* >() );
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      "test_gct",
      VK_MAKE_VERSION( 1, 0, 0 ),
      VK_MAKE_VERSION( 1, 2, 0 ),
      iext,
      /*std::vector< const char* >{
        VK_KHR_DISPLAY_EXTENSION_NAME,
        VK_KHR_SURFACE_EXTENSION_NAME
      },*/
      std::vector< const char* >{
        "VK_LAYER_KHRONOS_validation"
      }
    )
  );
  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME
  } );
  std::cout << nlohmann::json( groups[ 0 ] ).dump( 2 ) << std::endl;

  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;

  gct::glfw_window window( width, height, "window title", false );
  window.set_on_closed( []( auto & ) { std::cout << "closed" << std::endl; } );
  gct::glfw::get().poll();
  auto surface = window.get_surface( *groups[ 0 ].devices[ 0 ] );
  std::cout << nlohmann::json( *surface ).dump( 2 ) << std::endl;
 
  std::vector< gct::queue_requirement_t > queue_requirements{
    gct::queue_requirement_t{
      vk::QueueFlagBits::eGraphics,
      0u,
      vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
      vk::QueueGlobalPriorityEXT(),
#endif
      {/* **surface*/ },
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer
    },
    gct::queue_requirement_t{
      vk::QueueFlagBits::eTransfer,
      0u,
      vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
      vk::QueueGlobalPriorityEXT(),
#endif
      {},
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer
    }
  };
  auto device = selected.create_device(
    queue_requirements,
    gct::device_create_info_t()
  );
  auto queue = device->get_queue( 0u );
  auto gcb = queue->get_command_pool()->allocate();
  /*
  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();
  std::cout << "swapchain images : " << swapchain_images.size() << std::endl;
*/
  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 400 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 400 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 400 )
      .rebuild_chain()
  );

  auto pipeline_cache = device->get_pipeline_cache();
  
  VmaAllocatorCreateInfo allocator_create_info{};
  allocator_create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
  auto allocator = device->get_allocator(
    allocator_create_info
  );

  auto asbgi = gct::acceleration_structure_build_geometry_info_t()
    .set_basic(
      vk::AccelerationStructureBuildGeometryInfoKHR()
        .setType(
          vk::AccelerationStructureTypeKHR::eBottomLevel
        )
        .setFlags(
          vk::BuildAccelerationStructureFlagBitsKHR::eAllowUpdate
        )
        .setMode(
          vk::BuildAccelerationStructureModeKHR::eBuild
        )
    )
    .add_geometry(
      gct::acceleration_structure_geometry_t()
        .set_triangle(
          gct::acceleration_structure_geometry_triangles_data_t()
            .set_basic(
              vk::AccelerationStructureGeometryTrianglesDataKHR()
                .setVertexFormat( vk::Format::eR32G32B32Sfloat )
                .setMaxVertex( 300000 )
                .setIndexType( vk::IndexType::eUint32 )
            )
        )
    );
  std::vector< std::uint32_t > max_primitive_count{ 100000 };

  auto size = device->get_acceleration_structure_build_size(
    vk::AccelerationStructureBuildTypeKHR::eDevice,
    asbgi,
    max_primitive_count
  );
  std::cout << nlohmann::json( size ).dump( 2 ) << std::endl;
  auto as = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( size.get_basic().accelerationStructureSize )
          .setUsage( vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  )->create_acceleration_structure(
    vk::AccelerationStructureTypeKHR::eBottomLevel
  );
  std::cout << nlohmann::json( *as ).dump( 2 ) << std::endl;
  auto asa = as->get_address();
  std::cout << nlohmann::json( *asa ).dump( 2 ) << std::endl;
  auto as2 = allocator->create_buffer(
    gct::buffer_create_info_t()
      .set_basic(
        vk::BufferCreateInfo()
          .setSize( size.get_basic().accelerationStructureSize )
          .setUsage( vk::BufferUsageFlagBits::eStorageBuffer | vk::BufferUsageFlagBits::eShaderDeviceAddress )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  auto as2a = as2->get_address();
  std::cout << nlohmann::json( *as2a ).dump( 2 ) << std::endl;


}

