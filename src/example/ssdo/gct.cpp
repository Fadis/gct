#include <iostream>
#include <unordered_set>
#include <boost/program_options.hpp>
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
#include <gct/sampler_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/render_pass_begin_info.hpp>
#include <gct/submit_info.hpp>
#include <gct/shader_module.hpp>
#include <gct/fence.hpp>
#include <gct/wait_for_sync.hpp>
#include <gct/present_info.hpp>
#include <gct/gltf.hpp>
#include <gct/color.hpp>
#include <gct/timer.hpp>
#include <gct/cubemap.hpp>
#include <gct/gbuffer.hpp>
#include <gct/hysteresis.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  gct::render_pass_begin_info_t render_pass_begin_info;
  bool initial = true;
};

struct tone_state_t {
  std::uint32_t max;
  float scale;
};

int main( int argc, const char *argv[] ) {
  namespace po = boost::program_options;
  po::options_description desc( "Options" );
  desc.add_options()
    ( "help,h", "show this message" )
    ( "walk,w", po::value< std::string >()->default_value(".walk"), "walk state filename" )
    ( "model,m", po::value< std::string >(), "glTF filename" )
    ( "ambient,a", po::value< float >()->default_value( 0.1 ), "ambient light level" );
  po::variables_map vm;
  po::store( po::parse_command_line( argc, argv, desc ), vm );
  po::notify( vm );
  if( vm.count( "help" ) || !vm.count( "model" ) ) {
    std::cout << desc << std::endl;
    return 0;
  }
  const std::string walk_state_filename = vm[ "walk" ].as< std::string >();
  const std::string model_filename = vm[ "model" ].as< std::string >();
  const float ambient_level = std::min( std::max( vm[ "ambient" ].as< float >(), 0.f ), 1.f );

  gct::glfw::get();
  uint32_t iext_count = 0u;
  auto exts = glfwGetRequiredInstanceExtensions( &iext_count );
  std::vector< const char* > iext{};
  for( uint32_t i = 0u; i != iext_count; ++i )
    iext.push_back( exts[ i ] );
  iext.push_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );
  const auto ilayers = gct::get_instance_layers();
  const auto iexts = gct::get_instance_extensions( std::vector< const char* >() );
  std::shared_ptr< gct::instance_t > instance(
    new gct::instance_t(
      gct::instance_create_info_t()
        .set_application_info(
          vk::ApplicationInfo()
            .setPApplicationName( "my_application" )
            .setApplicationVersion(  VK_MAKE_VERSION( 1, 0, 0 ) )
            .setApiVersion( VK_MAKE_VERSION( 1, 3, 0 ) )
        )
        .add_layer(
          "VK_LAYER_KHRONOS_validation"
        )
        .add_extension(
          iext.begin(), iext.end()
        )
    )
  );
  instance->set_debug_callback(
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose|
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eInfo|
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning|
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
    vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
    [](
      vk::DebugUtilsMessageSeverityFlagBitsEXT,
      vk::DebugUtilsMessageTypeFlagsEXT,
      const vk::DebugUtilsMessengerCallbackDataEXT &data
    ) {
      std::cout << "validation : " << data.pMessage << std::endl;
      std::abort();
    }
  );

  auto groups = instance->get_physical_devices( {} );
  auto selected = groups[ 0 ].with_extensions( {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
    VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
    VK_KHR_MAINTENANCE1_EXTENSION_NAME,
    VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
  } );
  std::cout << nlohmann::json( groups[ 0 ] ).dump( 2 ) << std::endl;

  std::uint32_t width = 1920u*2;
  std::uint32_t height = 1080u*2;

  gct::glfw_window window( width, height, "window title", false );
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
      { **surface },
      vk::CommandPoolCreateFlagBits::eResetCommandBuffer
    }
  };
  auto device = selected.create_device(
    queue_requirements,
    gct::device_create_info_t()
  );
  auto queue = device->get_queue( 0u );
  auto gcb = queue->get_command_pool()->allocate();

  auto swapchain = device->get_swapchain( surface );
  auto swapchain_images = swapchain->get_images();
  std::cout << "swapchain images : " << swapchain_images.size() << std::endl;

  auto descriptor_pool = device->get_descriptor_pool(
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 250 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 2 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 9 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 8 )
      .rebuild_chain()
  );

  auto pipeline_cache = device->get_pipeline_cache();

  auto shadow_render_pass = device->get_render_pass(
    gct::render_pass_create_info_t()
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eR32G32Sfloat )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
          .setStencilLoadOp( vk::AttachmentLoadOp::eDontCare )
          .setStencilStoreOp( vk::AttachmentStoreOp::eDontCare )
          .setInitialLayout( vk::ImageLayout::eUndefined )
          .setFinalLayout( vk::ImageLayout::eShaderReadOnlyOptimal )
      )
      .add_attachment(
        vk::AttachmentDescription()
          .setFormat( vk::Format::eD32Sfloat )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setLoadOp( vk::AttachmentLoadOp::eClear )
          .setStoreOp( vk::AttachmentStoreOp::eStore )
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
  );

  VmaAllocatorCreateInfo allocator_create_info{};
  auto allocator = device->get_allocator(
    allocator_create_info
  );

  const unsigned int shadow_map_size = 1024u;
  std::vector< gct::cubemap_images > cubemap_images;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    cubemap_images.emplace_back( gct::cubemap_images(
      allocator,
      {
        gct::image_create_info_t()
          .set_basic(
            vk::ImageCreateInfo()
              .setImageType( vk::ImageType::e2D )
              .setFormat( vk::Format::eR32G32Sfloat )
              .setExtent(
                vk::Extent3D()
                  .setWidth( shadow_map_size )
                  .setHeight( shadow_map_size )
                  .setDepth( 1 )
              )
              .setUsage(
                vk::ImageUsageFlagBits::eColorAttachment |
                vk::ImageUsageFlagBits::eSampled |
                vk::ImageUsageFlagBits::eTransferSrc
              )
          ),
        gct::image_create_info_t()
          .set_basic(
            vk::ImageCreateInfo()
              .setImageType( vk::ImageType::e2D )
              .setFormat( vk::Format::eD32Sfloat )
              .setExtent(
                vk::Extent3D()
                  .setWidth( shadow_map_size )
                  .setHeight( shadow_map_size )
                  .setDepth( 1 )
              )
              .setUsage(
                vk::ImageUsageFlagBits::eDepthStencilAttachment |
                vk::ImageUsageFlagBits::eSampled
              )
          )
      },
      shadow_render_pass,
      std::vector< vk::ClearValue >{
        vk::ClearColorValue( gct::color::web::white ),
        vk::ClearDepthStencilValue( 1.f, 0 )
      }
    ) );
  }
  auto cubemap_sampler = device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  constexpr std::size_t gbuf_count = 8u;
  gct::gbuffer gbuffer(
    allocator,
    width,
    height,
    swapchain_images.size(),
    gbuf_count
  );
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( gbuffer.get_image_views(), vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }


  std::vector< fb_resources_t > framebuffers;

  const auto dynamic_descriptor_set_layout = device->get_descriptor_set_layout(
    gct::descriptor_set_layout_create_info_t()
      .add_binding(
        vk::DescriptorSetLayoutBinding()
          .setBinding( 0 )
          .setDescriptorType( vk::DescriptorType::eUniformBuffer )
          .setDescriptorCount( 1u )
          .setStageFlags( vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment )
      )
      .add_binding(
        vk::DescriptorSetLayoutBinding()
          .setBinding( 1 )
          .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          .setDescriptorCount( 1u )
          .setStageFlags( vk::ShaderStageFlagBits::eVertex|vk::ShaderStageFlagBits::eFragment )
      )
  );
  std::vector< std::shared_ptr< gct::buffer_t > > staging_dynamic_uniform;
  std::vector< std::shared_ptr< gct::buffer_t > > dynamic_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > dynamic_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    staging_dynamic_uniform.emplace_back(
      allocator->create_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      )
    );
    dynamic_uniform.emplace_back(
      allocator->create_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eUniformBuffer,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
    dynamic_descriptor_set.push_back(
      descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )
    );
    std::vector< gct::write_descriptor_set_t > updates;
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          vk::WriteDescriptorSet()
            .setDstSet( **dynamic_descriptor_set.back() )
            .setDstBinding( 0u )
            .setDescriptorCount( 1u )
            .setDescriptorType( vk::DescriptorType::eUniformBuffer )
        )
        .add_buffer( dynamic_uniform.back() )
    );
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          vk::WriteDescriptorSet()
            .setDstSet( **dynamic_descriptor_set.back() )
            .setDstBinding( 1u )
            .setDescriptorCount( 1u )
            .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
        )
        .add_image(
          gct::descriptor_image_info_t()
            .set_sampler( cubemap_sampler )
            .set_image_view( cubemap_images[ i ].get_cube_image_views( 0 ) )
            .set_basic(
              vk::DescriptorImageInfo()
                .setImageLayout(
                  vk::ImageLayout::eShaderReadOnlyOptimal
                )
            )
        )
    );
    dynamic_descriptor_set.back()->update( updates );
  }

  auto environment_sampler = device->get_sampler(
    gct::sampler_create_info_t()
      .set_basic(
        vk::SamplerCreateInfo()
          .setMagFilter( vk::Filter::eLinear )
          .setMinFilter( vk::Filter::eLinear )
          .setMipmapMode( vk::SamplerMipmapMode::eLinear )
          .setAddressModeU( vk::SamplerAddressMode::eRepeat )
          .setAddressModeV( vk::SamplerAddressMode::eRepeat )
          .setAddressModeW( vk::SamplerAddressMode::eRepeat )
          .setAnisotropyEnable( false )
          .setCompareEnable( false )
          .setMipLodBias( 0.f )
          .setMinLod( 0.f )
          .setMaxLod( 0.f )
          .setBorderColor( vk::BorderColor::eFloatTransparentBlack )
          .setUnnormalizedCoordinates( false )
      )
  );
  std::shared_ptr< gct::image_t > environment_image;
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      environment_image = recorder.load_image(
        allocator,
        "../images/environment.png",
        vk::ImageUsageFlagBits::eSampled,
        true, true
      );
      recorder.transfer_to_compute_barrier(
        {},
        { environment_image }
      );
    }
    command_buffer->execute_and_wait();
  }
  
  auto environment_image_view = environment_image->get_view(
    gct::image_view_create_info_t()
      .set_basic(
        vk::ImageViewCreateInfo()
          .setSubresourceRange(
            vk::ImageSubresourceRange()
              .setAspectMask( vk::ImageAspectFlagBits::eColor )
          )
          .setViewType( gct::to_image_view_type( environment_image->get_props().get_basic().imageType, environment_image->get_props().get_basic().arrayLayers ) )
          .setFormat( vk::Format::eR8G8B8A8Unorm )
      )
      .rebuild_chain()
  );

  const auto r32ici =
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eR32Sfloat )
          .setExtent( { width, height, 1 } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage
          )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      );
  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        vk::ImageCreateInfo()
          .setImageType( vk::ImageType::e2D )
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
          .setExtent( { width, height, 1 } )
          .setMipLevels( 1 )
          .setArrayLayers( 1 )
          .setSamples( vk::SampleCountFlagBits::e1 )
          .setTiling( vk::ImageTiling::eOptimal )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage
          )
          .setInitialLayout( vk::ImageLayout::eUndefined )
      );

  const auto [light_descriptor_set_layout,light_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv"
  );

  std::vector< std::shared_ptr< gct::image_view_t > > diffuse;
  std::vector< std::shared_ptr< gct::image_view_t > > specular;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    diffuse.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
    specular.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > light_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    light_descriptor_set.push_back(
      descriptor_pool->allocate(
        light_descriptor_set_layout
      )
    );
    light_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*light_descriptor_set.back())[ "gbuffer" ] )
          .add_image( gbuffer.get_image_view( i ) ),
        gct::write_descriptor_set_t()
          .set_basic( (*light_descriptor_set.back())[ "diffuse_image" ] )
          .add_image( diffuse[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*light_descriptor_set.back())[ "specular_image" ] )
          .add_image( specular[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*light_descriptor_set.back())[ "dynamic_uniforms" ] )
          .add_buffer( dynamic_uniform[ i ] )
      }
    );
  }

  const auto [ao_descriptor_set_layout,ao_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/ao/ssdo.comp.spv"
  );

  std::vector< std::shared_ptr< gct::image_view_t > > ao_out;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    ao_out.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( ao_out, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }


  std::vector< std::shared_ptr< gct::descriptor_set_t > > ao_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    ao_descriptor_set.push_back(
      descriptor_pool->allocate(
        ao_descriptor_set_layout
      )
    );
    ao_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*ao_descriptor_set.back())[ "gbuffer" ] )
          .add_image( gbuffer.get_image_view( i ) ),
        gct::write_descriptor_set_t()
          .set_basic( (*ao_descriptor_set.back())[ "dest_image" ] )
          .add_image( ao_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*ao_descriptor_set.back())[ "dynamic_uniforms" ] )
          .add_buffer( dynamic_uniform[ i ] ),
        /*gct::write_descriptor_set_t()
          .set_basic( (*ao_descriptor_set.back())[ "environment_map" ] )
          .add_image( environment_sampler, environment_image_view ),*/
        gct::write_descriptor_set_t()
          .set_basic(
            vk::WriteDescriptorSet()
              .setDstSet( **ao_descriptor_set.back() )
              .setDstBinding( 2u )
              .setDescriptorCount( 1u )
              .setDescriptorType( vk::DescriptorType::eCombinedImageSampler )
          )
          .add_image(
            gct::descriptor_image_info_t()
              .set_sampler( environment_sampler )
              .set_image_view( environment_image_view )
              .set_basic(
                vk::DescriptorImageInfo()
                  .setImageLayout(
                    environment_image->get_layout().get_uniform_layout()
                  )
              )
          )
      }
    );
  }

  const auto [ao_hgauss_descriptor_set_layout,ao_hgauss_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/selective_gauss/h12_32.comp.spv"
  );
  const auto [ao_vgauss_descriptor_set_layout,ao_vgauss_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/selective_gauss/v12_32.comp.spv"
  );

  std::vector< std::shared_ptr< gct::image_view_t > > ao_gauss_temp;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    ao_gauss_temp.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( ao_gauss_temp, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > ao_hgauss_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    ao_hgauss_descriptor_set.push_back(
      descriptor_pool->allocate(
        ao_hgauss_descriptor_set_layout
      )
    );
    ao_hgauss_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*ao_hgauss_descriptor_set.back())[ "gbuffer" ] )
          .add_image( gbuffer.get_image_view( i ) ),
        gct::write_descriptor_set_t()
          .set_basic( (*ao_hgauss_descriptor_set.back())[ "src_image" ] )
          .add_image( ao_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*ao_hgauss_descriptor_set.back())[ "dest_image" ] )
          .add_image( ao_gauss_temp[ i ] )
      }
    );
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > ao_vgauss_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    ao_vgauss_descriptor_set.push_back(
      descriptor_pool->allocate(
        ao_vgauss_descriptor_set_layout
      )
    );
    ao_vgauss_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*ao_vgauss_descriptor_set.back())[ "gbuffer" ] )
          .add_image( gbuffer.get_image_view( i ) ),
        gct::write_descriptor_set_t()
          .set_basic( (*ao_vgauss_descriptor_set.back())[ "src_image" ] )
          .add_image( ao_gauss_temp[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*ao_vgauss_descriptor_set.back())[ "dest_image" ] )
          .add_image( ao_out[ i ] )
      }
    );
  }

  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {

    framebuffers.emplace_back(
      fb_resources_t{
        device->get_semaphore(),
        device->get_semaphore(),
        device->get_semaphore(),
        queue->get_command_pool()->allocate(),
        gbuffer.get_render_pass_begin_info( i )
      }
    );
  }







  std::vector< std::shared_ptr< gct::buffer_t > > tone;
  std::vector< std::shared_ptr< gct::buffer_t > > tone_staging;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    tone.push_back(
      allocator->create_buffer(
        sizeof( tone_state_t ),
        vk::BufferUsageFlagBits::eStorageBuffer|
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
    tone_staging.push_back(
      allocator->create_buffer(
        sizeof( tone_state_t ),
        vk::BufferUsageFlagBits::eTransferDst|
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      )
    );
  }
  

  const auto [mix_ao_descriptor_set_layout,mix_ao_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv"
  );


  std::vector< std::shared_ptr< gct::image_view_t > > mixed_out;
  std::vector< std::shared_ptr< gct::image_view_t > > bloom_out;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    mixed_out.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
    bloom_out.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( mixed_out, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( bloom_out, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  std::vector< std::shared_ptr< gct::descriptor_set_t > > mix_ao_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    mix_ao_descriptor_set.push_back(
      descriptor_pool->allocate(
        mix_ao_descriptor_set_layout
      )
    );
    mix_ao_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "gbuffer" ] )
          .add_image( gbuffer.get_image_view( i ) ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "diffuse_image" ] )
          .add_image( diffuse[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "specular_image" ] )
          .add_image( specular[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "occlusion" ] )
          .add_image( ao_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "dest_image" ] )
          .add_image( mixed_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "bloom_image" ] )
          .add_image( bloom_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "tone" ] )
          .add_buffer( tone[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*mix_ao_descriptor_set.back())[ "dynamic_uniforms" ] )
          .add_buffer( dynamic_uniform[ i ] )
      }
    );
  }

  const auto [bloom_hgauss_descriptor_set_layout,bloom_hgauss_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/gauss/h12_32.comp.spv"
  );
  const auto [bloom_vgauss_descriptor_set_layout,bloom_vgauss_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/gauss/v12_32.comp.spv"
  );

  std::vector< std::shared_ptr< gct::image_view_t > > bloom_gauss_temp;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    bloom_gauss_temp.push_back(
      allocator->create_image(
        rgba32ici,
        VMA_MEMORY_USAGE_GPU_ONLY
      )->get_view( vk::ImageAspectFlagBits::eColor )
    );
  }
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( bloom_gauss_temp, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > bloom_hgauss_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    bloom_hgauss_descriptor_set.push_back(
      descriptor_pool->allocate(
        bloom_hgauss_descriptor_set_layout
      )
    );
    bloom_hgauss_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*bloom_hgauss_descriptor_set.back())[ "src_image" ] )
          .add_image( bloom_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*bloom_hgauss_descriptor_set.back())[ "dest_image" ] )
          .add_image( bloom_gauss_temp[ i ] )
      }
    );
  }
  std::vector< std::shared_ptr< gct::descriptor_set_t > > bloom_vgauss_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    bloom_vgauss_descriptor_set.push_back(
      descriptor_pool->allocate(
        bloom_vgauss_descriptor_set_layout
      )
    );
    bloom_vgauss_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*bloom_vgauss_descriptor_set.back())[ "src_image" ] )
          .add_image( bloom_gauss_temp[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*bloom_vgauss_descriptor_set.back())[ "dest_image" ] )
          .add_image( bloom_out[ i ] )
      }
    );
  }



  const auto [tone_descriptor_set_layout,tone_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/tone/tone.comp.spv"
  );

  std::vector< std::shared_ptr< gct::descriptor_set_t > > tone_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    tone_descriptor_set.push_back(
      descriptor_pool->allocate(
        tone_descriptor_set_layout
      )
    );
    tone_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*tone_descriptor_set.back())[ "src_image" ] )
          .add_image( mixed_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*tone_descriptor_set.back())[ "tone" ] )
          .add_buffer( tone[ i ] )
      }
    );
  }

  const auto [gamma_descriptor_set_layout,gamma_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv"
  );
  {
    auto command_buffer = queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( swapchain_images, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  std::vector< std::shared_ptr< gct::image_view_t > > swapchain_image_views;
  for( const auto &image: swapchain_images ) {
    swapchain_image_views.push_back( image->get_view( vk::ImageAspectFlagBits::eColor ) );
  }

  std::vector< std::shared_ptr< gct::descriptor_set_t > > gamma_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size(); ++i ) {
    gamma_descriptor_set.push_back(
      descriptor_pool->allocate(
        gamma_descriptor_set_layout
      )
    );
    gamma_descriptor_set.back()->update(
      {
        gct::write_descriptor_set_t()
          .set_basic( (*gamma_descriptor_set.back())[ "src_image" ] )
          .add_image( mixed_out[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*gamma_descriptor_set.back())[ "dest_image" ] )
          .add_image( swapchain_image_views[ i ] ),
        gct::write_descriptor_set_t()
          .set_basic( (*gamma_descriptor_set.back())[ "bloom_image" ] )
          .add_image( bloom_out[ i ] )
      }
    );
  }


  gct::gltf::document_t doc;
  {
    auto rec = gcb->begin();
    doc = gct::gltf::load_gltf(
      model_filename,
      device,
      rec,
      allocator,
      descriptor_pool,
      {
        shadow_render_pass,
        gbuffer.get_render_pass()
      },
      {
        CMAKE_CURRENT_BINARY_DIR "/shadow",
        CMAKE_CURRENT_BINARY_DIR "/geometry"
      },
      0,
      framebuffers.size(),
      0,
      float( width ) / float( height ),
      false,
      {
        dynamic_descriptor_set_layout
      }
    );
  }
  gcb->execute_and_wait();

  auto center = ( doc.node.min + doc.node.max ) / 2.f;
  auto scale = std::abs( glm::length( doc.node.max - doc.node.min ) );

  gct::cubemap_matrix cubemap_matrix( 
    glm::vec3{ 0.f, 0.f, 0.f },
    std::min( 0.1f*scale, 0.5f ),
    2.f*scale
  );

  auto const viewport =
    vk::Viewport()
      .setWidth( width )
      .setHeight( height )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const scissor( vk::Offset2D(0, 0), vk::Extent2D( width, height ) );
  auto const cube_viewport =
    vk::Viewport()
      .setWidth( shadow_map_size )
      .setHeight( shadow_map_size )
      .setMinDepth( 0.0f )
      .setMaxDepth( 1.0f );
  vk::Rect2D const cube_scissor( vk::Offset2D(0, 0), vk::Extent2D( shadow_map_size, shadow_map_size ) );


  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(width)/float(height)), std::min(0.1f*scale,0.5f), 150.f*scale );
  const float light_size = 0.3;

  gct::glfw_walk walk( center, scale, walk_state_filename );
  const auto point_lights = gct::gltf::get_point_lights(
    doc.node,
    doc.point_light
  );
  if( !point_lights.empty() ) {
    walk.set_light_energy( point_lights[ 0 ].intensity / ( 4 * M_PI ) / 100 );
    walk.set_light_pos( point_lights[ 0 ].location );
  }
  window.set_on_key(
    [&walk]( gct::glfw_window &p, int key, int scancode, int action, int mods ) {
      walk( p, key, scancode, action, mods );
    }
  );
  window.set_on_closed(
    [&walk]( auto & ) {
      walk.set_end();
    }
  );

  std::vector< std::shared_ptr< gct::buffer_t > > staging_cube_uniform;
  std::vector< std::shared_ptr< gct::buffer_t > > cube_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > cube_descriptor_set;
  for( std::size_t i = 0u; i != swapchain_images.size() * 6u; ++i ) {
    staging_cube_uniform.emplace_back(
      allocator->create_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eTransferSrc,
        VMA_MEMORY_USAGE_CPU_TO_GPU
      )
    );
    cube_uniform.emplace_back(
      allocator->create_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eTransferDst|vk::BufferUsageFlagBits::eUniformBuffer,
        VMA_MEMORY_USAGE_GPU_ONLY
      )
    );
    cube_descriptor_set.push_back(
      descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )
    );
    std::vector< gct::write_descriptor_set_t > updates;
    updates.push_back(
      gct::write_descriptor_set_t()
        .set_basic(
          vk::WriteDescriptorSet()
            .setDstSet( **cube_descriptor_set.back() )
            .setDstBinding( 0u )
            .setDescriptorCount( 1u )
            .setDescriptorType( vk::DescriptorType::eUniformBuffer )
        )
        .add_buffer( cube_uniform.back() )
    );
    cube_descriptor_set.back()->update( updates );
  }
  
  gct::hysteresis< float > tone_scale(
    1.f, 10, 60
  );

  uint32_t current_frame = 0u;
  uint32_t last_image_index = framebuffers.size();
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;
    ++walk;

    auto &sync = framebuffers[ current_frame ];
    if( !sync.initial ) {
      sync.command_buffer->wait_for_executed();
    }
    else sync.initial = false;
    auto image_index = swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];
    {
      auto mapped = tone_staging[ image_index ]->map< tone_state_t >();
      tone_scale.set( mapped.begin()->max / 65536.f );
      mapped.begin()->max = 0u;
      mapped.begin()->scale = tone_scale.get();
    }

    {
      auto rec = sync.command_buffer->begin();
      cubemap_matrix.move_center( walk.get_light_pos() );

      for( unsigned int i = 0u; i != 6u; ++i ) {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_projection_matrix( cubemap_matrix.get_projection_matrix() )
          .set_camera_matrix( cubemap_matrix.get_view_matrix( i ) )
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          staging_cube_uniform[ i + image_index * 6u ],
          cube_uniform[ i + image_index * 6u ]
        );
        rec.barrier(
          vk::AccessFlagBits::eTransferRead,
          vk::AccessFlagBits::eShaderRead,
          vk::PipelineStageFlagBits::eTransfer,
          vk::PipelineStageFlagBits::eVertexShader,
          vk::DependencyFlagBits( 0 ),
          { cube_uniform[ i + image_index * 6u ] },
          {}
        );
        auto render_pass_token = rec.begin_render_pass(
          cubemap_images[ image_index ].get_render_pass_begin_info( i ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &cube_viewport );
        rec->setScissor( 0, 1, &cube_scissor );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          0u,
          {
            cube_descriptor_set[ i + image_index * 6u ]
          }
        );
      }
      rec.barrier(
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::DependencyFlagBits( 0 ),
        {},
        { cubemap_images[ image_index ].get_image( 0 ) }
      );

      rec.copy(
        tone_staging[ image_index ],
        tone[ image_index ]
      );
      auto dynamic_data = gct::gltf::dynamic_uniforms_t()
        .set_projection_matrix( projection )
        .set_camera_matrix( walk.get_lookat() )
        .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
        .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
        .set_light_energy( walk.get_light_energy() )
        .set_light_size( light_size )
        .set_ambient( ambient_level );
      rec.copy(
        dynamic_data,
        staging_dynamic_uniform[ image_index ],
        dynamic_uniform[ image_index ]
      );
      rec.barrier(
        vk::AccessFlagBits::eTransferRead,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexShader,
        vk::DependencyFlagBits( 0 ),
        { dynamic_uniform[ image_index ] },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          fb.render_pass_begin_info,
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &viewport );
        rec->setScissor( 0, 1, &scissor );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          1u,
          {
            dynamic_descriptor_set[ image_index ]
          }
        );
      }
      rec.convert_image(
        gbuffer.get_image( image_index ),
        vk::ImageLayout::eGeneral
      );
      
      rec.bind(
        light_pipeline,
        { light_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );

      rec.bind(
        ao_pipeline,
        { ao_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );

      rec.compute_barrier(
        {},
        { ao_out[ image_index ]->get_factory() }
      );

      rec.bind(
        ao_hgauss_pipeline,
        { ao_hgauss_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        { ao_gauss_temp[ image_index ]->get_factory() }
      );
      rec.bind(
        ao_vgauss_pipeline,
        { ao_vgauss_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        {
          ao_out[ image_index ]->get_factory(),
          diffuse[ image_index ]->get_factory(),
          specular[ image_index ]->get_factory()
        }
      );
      rec.bind(
        mix_ao_pipeline,
        { mix_ao_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        {
          mixed_out[ image_index ]->get_factory(),
          bloom_out[ image_index ]->get_factory()
        }
      );
      rec.bind(
        bloom_hgauss_pipeline,
        { bloom_hgauss_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        { bloom_gauss_temp[ image_index ]->get_factory() }
      );
      rec.bind(
        bloom_vgauss_pipeline,
        { bloom_vgauss_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_barrier(
        {},
        { bloom_out[ image_index ]->get_factory() }
      );
      rec.transfer_to_compute_barrier(
        { tone[ image_index ] },
        {}
      );
      rec.bind(
        tone_pipeline,
        { tone_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.compute_to_transfer_barrier(
        { tone[ image_index ] },
        {}
      );
      rec.copy(
        tone[ image_index ],
        tone_staging[ image_index ]
      );
      rec.convert_image(
        swapchain_images[ image_index ],
        vk::ImageLayout::eGeneral
      );
      rec.bind(
        gamma_pipeline,
        { gamma_descriptor_set[ image_index ] }
      );
      rec.dispatch_threads( width, height, 1 );
      rec.convert_image(
        swapchain_images[ image_index ],
        vk::ImageLayout::ePresentSrcKHR
      );
    }
    sync.command_buffer->execute(
      gct::submit_info_t()
        .add_wait_for( sync.image_acquired, vk::PipelineStageFlagBits::eColorAttachmentOutput )
        .add_signal_to( sync.draw_complete )
    );
    queue->present(
      gct::present_info_t()
        .add_wait_for( sync.draw_complete )
        .add_swapchain( swapchain, image_index )
    );
    last_image_index = image_index;
    glfwPollEvents();
    ++current_frame;
    current_frame %= framebuffers.size();
  }
  (*queue)->waitIdle();
  walk.save( walk_state_filename );
}

