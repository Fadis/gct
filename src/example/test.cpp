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
#include <gct/device_address.hpp>
#include <gct/acceleration_structure.hpp>
#include <gct/acceleration_structure_geometry.hpp>
#include <gct/acceleration_structure_build_geometry_info.hpp>
#include <gct/acceleration_structure_geometry_triangles_data.hpp>
#include <gct/generate_blas.hpp>
#include <gct/generate_tlas.hpp>
#include <vulkan2json/AccelerationStructureInstanceKHR.hpp>

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
    VK_KHR_RAY_TRACING_PIPELINE_EXTENSION_NAME,
    VK_KHR_ACCELERATION_STRUCTURE_EXTENSION_NAME,
    VK_KHR_DEFERRED_HOST_OPERATIONS_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );

  std::uint32_t width = 1920u;
  std::uint32_t height = 1080u;

  gct::glfw_window window( width, height, "window title", false );
  window.set_on_closed( []( auto & ) { std::cout << "closed" << std::endl; } );
  gct::glfw::get().poll();
  auto surface = window.get_surface( *groups[ 0 ].devices[ 0 ] );
 
  std::vector< gct::queue_requirement_t > queue_requirements{
    gct::queue_requirement_t{
      vk::QueueFlagBits::eGraphics,
      0u,
      vk::Extent3D(),
#ifdef VK_EXT_GLOBAL_PRIORITY_EXTENSION_NAME
      vk::QueueGlobalPriorityEXT(),
#endif
      { **surface },
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
          .setMaxSets( 10000 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 400 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 4000 )
      .rebuild_chain()
  );
  
  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();

  auto pipeline_cache = device->get_pipeline_cache();

  std::vector< std::shared_ptr< gct::render_pass_t > > render_pass;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    render_pass.emplace_back( device->get_render_pass(
      gct::render_pass_create_info_t()
        .add_attachment(
          vk::AttachmentDescription()
            .setFormat( surface->get_caps().get_formats()[ 0 ].basic.format )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setLoadOp( vk::AttachmentLoadOp::eClear )
            .setStoreOp( vk::AttachmentStoreOp::eStore )
            .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
            .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
            .setInitialLayout( vk::ImageLayout::eUndefined )
            .setFinalLayout( vk::ImageLayout::ePresentSrcKHR )
        )
        .add_attachment(
          vk::AttachmentDescription()
            .setFormat( vk::Format::eD16Unorm )
            .setSamples( vk::SampleCountFlagBits::e1 )
            .setLoadOp( vk::AttachmentLoadOp::eClear )
            .setStoreOp( vk::AttachmentStoreOp::eDontCare )
            .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
            .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
            .setInitialLayout( vk::ImageLayout::eUndefined )
            .setFinalLayout( vk::ImageLayout::eDepthStencilAttachmentOptimal )
        )
        .add_subpass(
          gct::subpass_description_t()
            .add_color_attachment( 0, vk::ImageLayout::eColorAttachmentOptimal )
            .set_depth_stencil_attachment( 1, vk::ImageLayout::eDepthStencilAttachmentOptimal )
            .rebuild_chain()
        )
        .rebuild_chain()
    ) );
  }

  VmaAllocatorCreateInfo allocator_create_info{};
  allocator_create_info.flags = VMA_ALLOCATOR_CREATE_BUFFER_DEVICE_ADDRESS_BIT;
  auto allocator = device->get_allocator(
    allocator_create_info
  );
  
  std::vector< std::shared_ptr< gct::buffer_t > > staging_dynamic_uniform;
  std::vector< std::shared_ptr< gct::buffer_t > > dynamic_uniform;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    staging_dynamic_uniform.emplace_back(
      allocator->create_buffer(
        gct::buffer_create_info_t()
          .set_basic(
            vk::BufferCreateInfo()
              .setSize( sizeof( gct::gltf::dynamic_uniforms_t ) )
              .setUsage( vk::BufferUsageFlagBits::eTransferSrc )
          ),
          VMA_MEMORY_USAGE_CPU_TO_GPU
       )
    );
    dynamic_uniform.emplace_back(
      allocator->create_buffer(
        gct::buffer_create_info_t()
          .set_basic(
            vk::BufferCreateInfo()
              .setSize( sizeof( gct::gltf::dynamic_uniforms_t ) )
              .setUsage( vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eUniformBuffer )
          ),
          VMA_MEMORY_USAGE_GPU_ONLY
       )
    );
  }

  auto gcb = queue->get_command_pool()->allocate();
  gct::gltf::document_t doc;
  std::uint32_t instance_id = 0u;
  std::vector< std::shared_ptr< gct::acceleration_structure_t > > blas;
  std::vector< std::shared_ptr< gct::buffer_t > > blas_buf;
  std::vector< std::uint32_t > mesh2instance;
  std::shared_ptr< gct::acceleration_structure_t > tlas;
  {
    auto rec = gcb->begin();
    //rec.load_image( allocator, "/home/fadis/gltf/BoomBox/glTF/BoomBox_baseColor.png", vk::ImageUsageFlagBits::eSampled, true, false );
    doc = gct::gltf::load_gltf(
      "/home/fadis/gltf/Sponza/glTF/Sponza.gltf",
      device,
      rec,
      allocator,
      descriptor_pool,
      render_pass,
      "/home/fadis/git/gct/shaders",
      0,
      swapchain_images.size(),
      0,
      dynamic_uniform,
      float( width ) / float( height )
    );
    std::tie( blas, blas_buf, mesh2instance ) = gct::gltf::generate_blas(
      doc,
      rec,
      allocator
    );
    tlas = gct::gltf::generate_tlas(
      doc,
      rec,
      allocator,
      blas,
      blas_buf,
      mesh2instance
    );
  }
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb->execute(
    gct::submit_info_t()
  );
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb->wait_for_executed();
  std::cout << __FILE__ << " " << __LINE__ << std::endl;




  std::vector< std::uint32_t > max_primitive_count{ 100000 };
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
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
                ///
            )
        )
    );

  auto size = device->get_acceleration_structure_build_size(
    vk::AccelerationStructureBuildTypeKHR::eDevice,
    asbgi,
    max_primitive_count
  );
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  std::cout << nlohmann::json( size ).dump( 2 ) << std::endl;
  {
    auto as = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().accelerationStructureSize )
            .setUsage(
              vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR|
              vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
            )
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
    auto deferred = device->get_deferred_operation();
  }
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
}

