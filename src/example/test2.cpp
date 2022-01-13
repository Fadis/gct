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
#include <gct/shader_module.hpp>
#include <gct/device_address.hpp>
#include <gct/acceleration_structure.hpp>
#include <gct/acceleration_structure_geometry.hpp>
#include <gct/acceleration_structure_build_geometry_info.hpp>
#include <gct/acceleration_structure_geometry_triangles_data.hpp>
#include <gct/strided_device_address_region.hpp>
#include <gct/ray_tracing_pipeline_create_info.hpp>
#include <gct/ray_tracing_pipeline.hpp>
#include <gct/generate_blas.hpp>
#include <gct/generate_tlas.hpp>
#include <gct/round_up.hpp>
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
  auto &rtprops = selected.devices[ 0 ]->get_props().get_ray_tracing_pipeline();

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
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 400 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 400 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 4000 )
      .set_descriptor_pool_size( vk::DescriptorType::eAccelerationStructureKHR, 400 )
      .rebuild_chain()
  );
  
  const auto rgen = device->get_shader_module(
    "../shaders/simple.rgen.spv"
  );
  const auto rchit = device->get_shader_module(
    "../shaders/simple.rchit.spv"
  );
  const auto rmiss = device->get_shader_module(
    "../shaders/simple.rmiss.spv"
  );
  const auto rmiss2 = device->get_shader_module(
    "../shaders/simple2.rmiss.spv"
  );
  
  const auto descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        rgen->get_props().get_reflection()
      )
      .add_binding(
        rmiss->get_props().get_reflection()
      )
      .add_binding(
        rmiss2->get_props().get_reflection()
      )
      .add_binding(
        rchit->get_props().get_reflection()
      )
      .rebuild_chain()
  );
 
  const auto descriptor_set = descriptor_pool->allocate( descriptor_set_layout );

  std::cout << nlohmann::json( *descriptor_set_layout ).dump( 2 ) << std::endl; 

  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();

  auto pipeline_cache = device->get_pipeline_cache();

  auto pipeline_layout = device->get_pipeline_layout(
    gct::pipeline_layout_create_info_t()
      .add_descriptor_set_layout( descriptor_set_layout )
      .add_push_constant_range(
        vk::PushConstantRange()
          .setStageFlags( vk::ShaderStageFlagBits::eRaygenKHR )
          .setOffset( 0 )
          .setSize( sizeof( glm::mat4 ) )
      )
  );

  auto ray_tracing_pipeline = pipeline_cache->get_pipeline(
    gct::ray_tracing_pipeline_create_info_t()
    .add_stage( rgen )
    .add_stage( rmiss )
    .add_stage( rmiss2 )
    .add_stage( rchit )
    .set_layout( pipeline_layout )
  );

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
 
  const auto dest_buffer = allocator->create_pixel_buffer(
    vk::BufferUsageFlagBits::eTransferDst,
    VMA_MEMORY_USAGE_GPU_TO_CPU,
    { 1920, 1080, 1 },
    vk::Format::eR8G8B8A8Unorm
  );


  auto dest_image = allocator->create_image(
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eR8G8B8A8Unorm )
          .setExtent( { 1920, 1080, 1 } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage(
            vk::ImageUsageFlagBits::eTransferSrc |
            vk::ImageUsageFlagBits::eStorage
          )
          .setSharingMode( vk::SharingMode::eExclusive )
          .setQueueFamilyIndexCount( 0 )
          .setPQueueFamilyIndices( nullptr )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      ),
      VMA_MEMORY_USAGE_GPU_ONLY
  );

  auto dest_view = 
    dest_image->get_view(
      gct::image_view_create_info_t()
        .set_basic(
          vk::ImageViewCreateInfo()
            .setSubresourceRange(
              vk::ImageSubresourceRange()
                .setAspectMask( vk::ImageAspectFlagBits::eColor )
                .setBaseMipLevel( 0 )
                .setLevelCount( 1 )
                .setBaseArrayLayer( 0 )
                .setLayerCount( 1 )
            )
            .setViewType( gct::to_image_view_type( dest_image->get_props().get_basic().imageType ) )
        )
        .rebuild_chain()
    );

  struct Vertex {
    float pos[3];
  };
  std::vector<Vertex> vertex_data = {
    { {  1.0f,  1.0f, 0.0f } },
    { { -1.0f,  1.0f, 0.0f } },
    { {  0.0f, -1.0f, 0.0f } }
  };

  std::vector<uint32_t> index_data = { 0, 1, 2 };
  auto index_count = static_cast<uint32_t>(index_data.size());

  vk::TransformMatrixKHR transform_data{
    std::array< std::array< float, 4 >, 3 >{
      std::array< float, 4 >{ 1.0f, 0.0f, 0.0f, 0.0f },
        std::array< float, 4 >{ 0.0f, 1.0f, 0.0f, 0.0f },
        std::array< float, 4 >{ 0.0f, 0.0f, 1.0f, 0.0f }
    }
  };

  auto gcb = queue->get_command_pool()->allocate();
  std::shared_ptr< gct::buffer_t > vertex;
  std::shared_ptr< gct::buffer_t > index;
  std::shared_ptr< gct::buffer_t > transform;
  std::shared_ptr< gct::buffer_t > blas_buffer;
  std::shared_ptr< gct::acceleration_structure_t > blas;
  {
    auto rec = gcb->begin();
    vertex = rec.load_buffer(
      allocator,
      vertex_data.data(),
      vertex_data.size() * sizeof(Vertex),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    index = rec.load_buffer(
      allocator,
      index_data.data(),
      index_data.size() * sizeof(std::uint32_t),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    transform = rec.load_buffer(
      allocator,
      index_data.data(),
      sizeof( vk::TransformMatrixKHR ),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );
    
    auto asbgi = gct::acceleration_structure_build_geometry_info_t()
      .set_basic(
        vk::AccelerationStructureBuildGeometryInfoKHR()
          .setType(
            vk::AccelerationStructureTypeKHR::eBottomLevel
          )
          .setFlags(
            vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace
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
                  .setIndexType( vk::IndexType::eUint32 )
                  .setVertexFormat( vk::Format::eR32G32B32Sfloat )
                  .setVertexStride( sizeof( Vertex ) )
                  .setMaxVertex( 3 )
              )
              .set_vertex_data(
                vertex->get_address()
              )
              .set_index_data(
                index->get_address()
              )
              .set_transform_data(
                transform->get_address()
              )
          )
      );
    std::vector< std::uint32_t > max_primitive_count{ 1 };
    auto size = device->get_acceleration_structure_build_size(
      vk::AccelerationStructureBuildTypeKHR::eDevice,
      asbgi,
      max_primitive_count
    );
    std::cout << __FILE__ << " " << __LINE__ << " " << nlohmann::json( size ).dump( 2 ) << std::endl;
    auto scratch_buffer_addr = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().buildScratchSize )
            .setUsage(
              vk::BufferUsageFlagBits::eStorageBuffer|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    )->get_address();
    blas_buffer = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().accelerationStructureSize )
            .setUsage(
              vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    blas = blas_buffer->create_acceleration_structure(
        vk::AccelerationStructureTypeKHR::eBottomLevel
      );
    asbgi
      .set_src( blas )
      .set_dst( blas )
      .set_scratch( scratch_buffer_addr )
      .rebuild_chain();
    std::cout << __FILE__ << " " << __LINE__ << " " << nlohmann::json( asbgi ).dump( 2 ) << std::endl;
    rec.build_acceleration_structure(
      asbgi,
      std::vector< vk::AccelerationStructureBuildRangeInfoKHR >{
        vk::AccelerationStructureBuildRangeInfoKHR()
          .setPrimitiveCount( 1 )
      }
    );
  }
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb->execute(
    gct::submit_info_t()
  );
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb->wait_for_executed();
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  
  auto gcb2 = queue->get_command_pool()->allocate();
  std::shared_ptr< gct::buffer_t > instance_buffer;
  std::shared_ptr< gct::buffer_t > tlas_buffer;
  std::shared_ptr< gct::acceleration_structure_t > tlas;
  {
    auto rec = gcb2->begin();

    std::vector< vk::AccelerationStructureInstanceKHR > instance_data;
    instance_data.push_back(
      vk::AccelerationStructureInstanceKHR()
        .setTransform( transform_data )
        .setInstanceCustomIndex( 0 )
        .setMask( 0xFF )
        .setInstanceShaderBindingTableRecordOffset( 0 )
        .setFlags( vk::GeometryInstanceFlagBitsKHR::eTriangleFacingCullDisable )
        .setAccelerationStructureReference( *blas->get_address() )
    );

    instance_buffer = rec.load_buffer(
      allocator,
      reinterpret_cast< const void* >( instance_data.data() ),
      sizeof( vk::AccelerationStructureInstanceKHR ) * instance_data.size(),
      vk::BufferUsageFlagBits::eAccelerationStructureBuildInputReadOnlyKHR
    );

    auto asbgi = gct::acceleration_structure_build_geometry_info_t()
      .set_basic(
        vk::AccelerationStructureBuildGeometryInfoKHR()
          .setType(
            vk::AccelerationStructureTypeKHR::eTopLevel
          )
          .setFlags(
            vk::BuildAccelerationStructureFlagBitsKHR::ePreferFastTrace
          )
          .setMode(
            vk::BuildAccelerationStructureModeKHR::eBuild
          )
      )
      .add_geometry(
        gct::acceleration_structure_geometry_t()
          .set_instance(
            gct::acceleration_structure_geometry_instances_data_t()
              .set_basic(
                vk::AccelerationStructureGeometryInstancesDataKHR()
                  .setArrayOfPointers( false )
              )
              .set_data( instance_buffer->get_address() )
          )
      );
    std::vector< std::uint32_t > max_primitive_count{ 1 };
    auto size = device->get_acceleration_structure_build_size(
      vk::AccelerationStructureBuildTypeKHR::eDevice,
      asbgi,
      max_primitive_count
    );
    std::cout << __FILE__ << " " << __LINE__ << " " << nlohmann::json( size ).dump( 2 ) << std::endl;
    auto scratch_buffer_addr = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().buildScratchSize )
            .setUsage(
              vk::BufferUsageFlagBits::eStorageBuffer|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    )->get_address();
    tlas_buffer = allocator->create_buffer(
      gct::buffer_create_info_t()
        .set_basic(
          vk::BufferCreateInfo()
            .setSize( size.get_basic().accelerationStructureSize )
            .setUsage(
              vk::BufferUsageFlagBits::eAccelerationStructureStorageKHR|
              vk::BufferUsageFlagBits::eShaderDeviceAddress
            )
        ),
      VMA_MEMORY_USAGE_GPU_ONLY
    );
    tlas = tlas_buffer->create_acceleration_structure(
        vk::AccelerationStructureTypeKHR::eTopLevel
      );
    asbgi
      .set_src( tlas )
      .set_dst( tlas )
      .set_scratch( scratch_buffer_addr )
      .rebuild_chain();
    std::cout << __FILE__ << " " << __LINE__ << " " << nlohmann::json( asbgi ).dump( 2 ) << std::endl;
    rec.build_acceleration_structure(
      asbgi,
      std::vector< vk::AccelerationStructureBuildRangeInfoKHR >{
        vk::AccelerationStructureBuildRangeInfoKHR()
          .setPrimitiveCount( 1 )
      }
    );
    rec.convert_image( dest_image, vk::ImageLayout::eUndefined, vk::ImageLayout::eGeneral );
  }
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb2->execute(
    gct::submit_info_t()
  );
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb2->wait_for_executed();
  std::cout << __FILE__ << " " << __LINE__ << std::endl;

  descriptor_set->update(
    {
      gct::write_descriptor_set_t()
        .set_basic(
          (*descriptor_set)[ "as" ]
        )
        .add_acceleration_structure_handle(
          tlas
        ),
      gct::write_descriptor_set_t()
        .set_basic(
          (*descriptor_set)[ "dest_image" ]
        )
        .add_image(
          gct::descriptor_image_info_t()
            .set_basic(
              vk::DescriptorImageInfo()
                .setImageLayout(
                  dest_image->get_props().get_basic().initialLayout
                )
            )
            .set_image_view( dest_view )
        )
    }
  );


  const std::uint32_t raygen_shader_count = 1u;
  const std::uint32_t miss_shader_count = 2u;
  const std::uint32_t hit_shader_count = 1u;

  const std::uint32_t raygen_shader_binding_offset = 0;
  const std::uint32_t raygen_shader_binding_stride = rtprops.shaderGroupHandleSize;
  const std::uint32_t raygen_shader_table_size = raygen_shader_count * raygen_shader_binding_stride;

  const std::uint32_t miss_shader_binding_offset =
    raygen_shader_binding_offset +
    gct::round_up( raygen_shader_table_size, rtprops.shaderGroupBaseAlignment );
  std::cout << __FILE__ << " " << __LINE__ << " "  << miss_shader_binding_offset << " " << raygen_shader_table_size << " " << rtprops.shaderGroupBaseAlignment << std::endl;
  const std::uint32_t miss_shader_binding_stride = rtprops.shaderGroupHandleSize;
  const std::uint32_t miss_shader_table_size = miss_shader_count * miss_shader_binding_stride;

  const std::uint32_t hit_shader_binding_offset =
    miss_shader_binding_offset +
    gct::round_up( miss_shader_table_size, rtprops.shaderGroupBaseAlignment );
  const std::uint32_t hit_shader_binding_stride = rtprops.shaderGroupHandleSize;
  const std::uint32_t hit_shader_table_size = hit_shader_count * hit_shader_binding_stride;

  const std::uint32_t shader_binding_table_size = hit_shader_binding_offset + hit_shader_table_size;

  std::vector< std::uint8_t > shader_binding_table_data( shader_binding_table_size );
  const auto get_raygen_handles_result = (*device)->getRayTracingShaderGroupHandlesKHR(
    **ray_tracing_pipeline,
    0u,
    raygen_shader_count,
    raygen_shader_table_size,
    std::next( shader_binding_table_data.data(), raygen_shader_binding_offset )
  );
  if( get_raygen_handles_result != vk::Result::eSuccess ) {
    vk::throwResultException( get_raygen_handles_result, "getRayTracingShaderGroupHandlesKHR failed" );
  }
  const auto get_miss_handles_result = (*device)->getRayTracingShaderGroupHandlesKHR(
    **ray_tracing_pipeline,
    raygen_shader_count,
    miss_shader_count,
    miss_shader_table_size,
    std::next( shader_binding_table_data.data(), miss_shader_binding_offset )
  );
  if( get_miss_handles_result != vk::Result::eSuccess ) {
    vk::throwResultException( get_miss_handles_result, "getRayTracingShaderGroupHandlesKHR failed" );
  }
  const auto get_hit_handles_result = (*device)->getRayTracingShaderGroupHandlesKHR(
    **ray_tracing_pipeline,
    raygen_shader_count + miss_shader_count,
    hit_shader_count,
    hit_shader_table_size,
    std::next( shader_binding_table_data.data(), hit_shader_binding_offset )
  );
  if( get_hit_handles_result != vk::Result::eSuccess ) {
    vk::throwResultException( get_hit_handles_result, "getRayTracingShaderGroupHandlesKHR failed" );
  }

  auto gcb3 = queue->get_command_pool()->allocate();
  std::shared_ptr< gct::buffer_t > shader_binding_table;
  {
    auto rec = gcb3->begin();
    std::cout << nlohmann::json( shader_binding_table_data ) << std::endl;
    shader_binding_table = rec.load_buffer(
      allocator,
      shader_binding_table_data,
      vk::BufferUsageFlagBits::eShaderBindingTableKHR
    );
  }
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb3->execute(
    gct::submit_info_t()
  );
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  gcb3->wait_for_executed();
  std::cout << __FILE__ << " " << __LINE__ << std::endl;


  auto draw_commands = queue->get_command_pool()->allocate();
  {
    auto rec = draw_commands->begin();
    rec.bind_descriptor_set(
      vk::PipelineBindPoint::eRayTracingKHR,
      pipeline_layout,
      descriptor_set
    );
    rec.bind_pipeline(
      vk::PipelineBindPoint::eRayTracingKHR,
      ray_tracing_pipeline
    );
    std::cout << __FILE__ << " " << __LINE__ << " " << hit_shader_binding_offset << std::endl;
    rec.trace_rays(
      shader_binding_table->get_address().get_strided(
        raygen_shader_binding_stride,
        raygen_shader_table_size
      ),
      ( shader_binding_table->get_address() + miss_shader_binding_offset ).get_strided(
        miss_shader_binding_stride,
        miss_shader_table_size
      ),
      ( shader_binding_table->get_address() + hit_shader_binding_offset ).get_strided(
        hit_shader_binding_stride,
        hit_shader_table_size
      ),
      gct::strided_device_address_region_t(),
      1920,
      1080,
      1
    );
    rec.barrier(
      vk::AccessFlagBits::eShaderWrite,
      vk::AccessFlagBits::eTransferRead,
      vk::PipelineStageFlagBits::eRayTracingShaderKHR,
      vk::PipelineStageFlagBits::eTransfer,
      vk::DependencyFlagBits( 0 ),
      {},
      { dest_image }
    );

    rec.copy(
      dest_image,
      dest_buffer
    );
  }
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  draw_commands->execute(
    gct::submit_info_t()
  );
  std::cout << __FILE__ << " " << __LINE__ << std::endl;
  draw_commands->wait_for_executed();
  std::cout << __FILE__ << " " << __LINE__ << std::endl;

  dest_buffer->dump_image( "out.png" );

}

