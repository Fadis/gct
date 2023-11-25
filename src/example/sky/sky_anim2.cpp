#include <iostream>
#include <glm/gtx/string_cast.hpp>
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
#include <gct/buffer.hpp>
#include <gct/mappable_buffer.hpp>
#include <gct/image_create_info.hpp>
#include <gct/image.hpp>
#include <gct/swapchain.hpp>
#include <gct/descriptor_pool.hpp>
#include <gct/descriptor_set_layout.hpp>
#include <gct/sampler_create_info.hpp>
#include <gct/pipeline_cache.hpp>
#include <gct/pipeline_layout_create_info.hpp>
#include <gct/pipeline_layout.hpp>
#include <gct/buffer_view_create_info.hpp>
#include <gct/compute_pipeline_create_info.hpp>
#include <gct/compute_pipeline.hpp>
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
#include <gct/command_buffer.hpp>
#include <gct/command_pool.hpp>
#include <gct/framebuffer.hpp>
#include <gct/render_pass.hpp>
#include <gct/cubemap_matrix_generator.hpp>
#include <gct/image_filter.hpp>
#include <gct/named_resource.hpp>
#include <gct/compute.hpp>
#include <gct/graphics.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/tone_mapping.hpp>
#include <gct/hbao.hpp>
#include <gct/device.hpp>
#include <gct/shader_module.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/graphics_pipeline_create_info.hpp>
#include <gct/pipeline_input_assembly_state_create_info.hpp>
#include <gct/pipeline_viewport_state_create_info.hpp>
#include <gct/pipeline_color_blend_state_create_info.hpp>
#include <gct/vertex_attributes.hpp>
#include <gct/voxel_image.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  gct::render_pass_begin_info_t render_pass_begin_info;
  bool initial = true;
};

struct generate_fog_parameters_t {
  LIBGCT_SETTER( screen_to_world )
  LIBGCT_SETTER( period )
  glm::mat4 screen_to_world;
  glm::vec4 period;
};

struct froxel_state_t {
  LIBGCT_SETTER( world_to_screen )
  LIBGCT_SETTER( screen_to_world )
  LIBGCT_SETTER( sigma_ma )
  LIBGCT_SETTER( sigma_oa )
  LIBGCT_SETTER( sigma_rs )
  LIBGCT_SETTER( sigma_ms )
  LIBGCT_SETTER( eye_pos )
  LIBGCT_SETTER( light_pos )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( altitude )
  LIBGCT_SETTER( light_energy )
  glm::mat4 world_to_screen;
  glm::mat4 screen_to_world;
  glm::vec4 sigma_ma;
  glm::vec4 sigma_oa;
  glm::vec4 sigma_rs;
  glm::vec4 sigma_ms;
  glm::vec4 eye_pos;
  glm::vec4 light_pos;
  float g;
  float ground_radius;
  float top_radius;
  float altitude;
  float light_energy;
};

struct transmittance_parameters_t {
  LIBGCT_SETTER( sigma_ma )
  LIBGCT_SETTER( sigma_oa )
  LIBGCT_SETTER( sigma_rs )
  LIBGCT_SETTER( sigma_ms )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  glm::vec4 sigma_ma;
  glm::vec4 sigma_oa;
  glm::vec4 sigma_rs;
  glm::vec4 sigma_ms;
  float ground_radius;
  float top_radius;
};

struct multiscat_parameters_t {
  LIBGCT_SETTER( sigma_ma )
  LIBGCT_SETTER( sigma_oa )
  LIBGCT_SETTER( sigma_rs )
  LIBGCT_SETTER( sigma_ms )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( light_energy )
  glm::vec4 sigma_ma;
  glm::vec4 sigma_oa;
  glm::vec4 sigma_rs;
  glm::vec4 sigma_ms;
  float g;
  float ground_radius;
  float top_radius;
  float light_energy;
};

struct skyview_parameters_t {
  LIBGCT_SETTER( sigma_ma )
  LIBGCT_SETTER( sigma_oa )
  LIBGCT_SETTER( sigma_rs )
  LIBGCT_SETTER( sigma_ms )
  LIBGCT_SETTER( light_pos )
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( ground_radius )
  LIBGCT_SETTER( top_radius )
  LIBGCT_SETTER( altitude )
  LIBGCT_SETTER( light_energy )
  glm::vec4 sigma_ma;
  glm::vec4 sigma_oa;
  glm::vec4 sigma_rs;
  glm::vec4 sigma_ms;
  glm::vec4 light_pos;
  float g;
  float ground_radius;
  float top_radius;
  float altitude;
  float light_energy;
};

int main( int argc, const char *argv[] ) {
  const gct::common_sample_setup res(
    argc, argv,
    {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
      VK_KHR_SWAPCHAIN_MUTABLE_FORMAT_EXTENSION_NAME,
      VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,
      VK_KHR_MAINTENANCE1_EXTENSION_NAME,
      VK_EXT_PIPELINE_CREATION_FEEDBACK_EXTENSION_NAME
    },
    gct::descriptor_pool_create_info_t()
      .set_basic(
        vk::DescriptorPoolCreateInfo()
          .setFlags( vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet )
          .setMaxSets( 250 )
      )
      .set_descriptor_pool_size( vk::DescriptorType::eUniformBuffer, 2 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageBuffer, 1 )
      .set_descriptor_pool_size( vk::DescriptorType::eCombinedImageSampler, 9 )
      .set_descriptor_pool_size( vk::DescriptorType::eStorageImage, 8 )
      .rebuild_chain()
  );

  const unsigned int transmittance_width = 256u;
  const unsigned int transmittance_height = 64u;
  const auto transmittance = res.allocator->create_image_views(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( transmittance_width, transmittance_height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    1u
  );
  const unsigned int multiscat_size = 32u;
  const auto multiscat = res.allocator->create_image_views(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( multiscat_size, multiscat_size )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    1u
  );
  const unsigned int skyview_width = 256u;
  const unsigned int skyview_height = 128u;
  const auto skyview = res.allocator->create_image_views(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( skyview_width, skyview_height )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled|
            vk::ImageUsageFlagBits::eTransferSrc
          )
          .setFormat( vk::Format::eR32G32B32A32Sfloat )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    1u
  );
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( transmittance, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( multiscat, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( skyview, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  auto linear_sampler = res.device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );
  
  const auto generate_transmittance = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/generate_transmittance.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "dest_image", transmittance } )
  );
  const auto generate_multiscat = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/generate_multiscat.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "transmittance", linear_sampler, transmittance, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", multiscat } )
  );
  const auto generate_skyview = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/generate_skyview.comp.spv" )
      .set_swapchain_image_count( 1 )
      .add_resource( { "transmittance", linear_sampler, transmittance, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "multiscattering", linear_sampler, multiscat, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", skyview } )
  );
  const glm::vec4 sigma_ma( 4.40e-6, 4.40e-6, 4.40e-6, 0.0 );
  const glm::vec4 sigma_oa( 0.650e-6, 1.881e-6, 0.085e-6, 0.0 );
  const glm::vec4 sigma_rs( 5.802e-6, 13.558e-6, 33.1e-6, 0.0 );
  const glm::vec4 sigma_ms( 3.996e-6, 3.996e-6, 3.996e-6, 0.0 );
  const float ground_radius = 6360000.f;
  const float top_radius = 6460000.f;
  const float light_energy = 20.f;
  const float g = 0.8f;
  float sun_angle = M_PI/3.0;
  glm::vec4 light_pos = glm::vec4( float( std::cos( sun_angle ) ), float( std::sin( sun_angle ) ), 0.f, 0.f ) * 14960000000.0f;
  glm::vec4 light_pos_near = glm::vec4( glm::vec3( float( std::cos( sun_angle ) ), float( std::sin( sun_angle ) ), 0.f ) * 149.6f, 1.0 );
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      {
        auto params = transmittance_parameters_t()
          .set_sigma_ma( sigma_ma )
          .set_sigma_oa( sigma_oa )
          .set_sigma_rs( sigma_rs )
          .set_sigma_ms( sigma_ms )
          .set_ground_radius( ground_radius )
          .set_top_radius( top_radius );
        rec->pushConstants(
          **generate_transmittance.get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( transmittance_parameters_t ),
          reinterpret_cast< void* >( &params )
        );
      }
      generate_transmittance( rec, 0u, transmittance_width, transmittance_height, 1u );
      rec.convert_image(
        transmittance[ 0 ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      {
        auto params = multiscat_parameters_t()
          .set_sigma_ma( sigma_ma )
          .set_sigma_oa( sigma_oa )
          .set_sigma_rs( sigma_rs )
          .set_sigma_ms( sigma_ms )
          .set_ground_radius( ground_radius )
          .set_top_radius( top_radius )
          .set_g( 0.8f )
          .set_light_energy( light_energy );
        rec->pushConstants(
          **generate_multiscat.get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( multiscat_parameters_t ),
          reinterpret_cast< void* >( &params )
        );
      }
      generate_multiscat( rec, 0u, multiscat_size, multiscat_size, 1u );
      rec.convert_image(
        multiscat[ 0 ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      /*rec.dump_image(
        res.allocator,
        transmittance[ 0 ]->get_factory(),
        "transmittance.jpg",
        0
      );
      rec.dump_image(
        res.allocator,
        multiscat[ 0 ]->get_factory(),
        "multiscat.jpg",
        0
      );*/
    }
    command_buffer->execute_and_wait();
  }
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      rec.convert_image(
        transmittance[ 0 ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      rec.convert_image(
        multiscat[ 0 ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      rec.convert_image(
        skyview[ 0 ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
    }
    command_buffer->execute_and_wait();
  }




  gct::cubemap_matrix_generator shadow_mat(
    res.allocator,
    res.descriptor_pool,
    res.pipeline_cache,
    CMAKE_CURRENT_BINARY_DIR "/shadow_mat/shadow_mat.comp.spv",
    res.swapchain_images.size()
  );

  const unsigned int shadow_map_size = 1024u;
  gct::gbuffer shadow_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( shadow_map_size )
      .set_height( shadow_map_size )
      .set_layer( 6 )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .set_color_buffer_count( 1 )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_format( vk::Format::eR32G32Sfloat )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );

  gct::cubemap_images2 cubemap_images( shadow_gbuffer.get_image_views() );
  
  auto cubemap_sampler = res.device->get_sampler(
    gct::get_basic_linear_sampler_create_info()
  );

  constexpr std::size_t gbuf_count = 8u;
  gct::gbuffer gbuffer(
    res.allocator,
    res.width,
    res.height,
    res.swapchain_images.size(),
    gbuf_count
  );

  const std::uint32_t froxel_xy_resolution = 32u;
  const std::uint32_t froxel_z_resolution = 32u;
  const auto froxel = res.allocator->create_image_views(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_3d_image( froxel_xy_resolution, froxel_xy_resolution, froxel_z_resolution )
          .setUsage(
            vk::ImageUsageFlagBits::eStorage|
            vk::ImageUsageFlagBits::eSampled
          )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );
  
  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );

  const auto rendered_froxel = res.allocator->create_image_views(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );
  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( froxel, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( rendered_froxel, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  std::vector< std::shared_ptr< gct::mappable_buffer_t > > froxel_state;
  for( std::size_t i = 0u; i != res.swapchain_images.size(); ++i ) {
    froxel_state.emplace_back(
      res.allocator->create_mappable_buffer(
        sizeof( froxel_state_t ),
        vk::BufferUsageFlagBits::eUniformBuffer
      )
    );
  }

  const auto calc_scattering = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/froxel.comp.spv" )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .add_resource( { "froxel_state", froxel_state } )
      .add_resource( { "transmittance", linear_sampler, transmittance, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "multiscattering", linear_sampler, multiscat, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "scattering", froxel, vk::ImageLayout::eGeneral } )
  );

  const auto render_scattering = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/skyview/render_froxel.comp.spv" )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .add_resource( { "froxel_state", froxel_state } )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "scattering", linear_sampler, froxel, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "dest_image", rendered_froxel, vk::ImageLayout::eGeneral } )
  );

  std::vector< fb_resources_t > framebuffers;

  const auto dynamic_descriptor_set_layout = res.device->get_descriptor_set_layout(
    {       
      CMAKE_CURRENT_BINARY_DIR "/geometry",
      CMAKE_CURRENT_BINARY_DIR "/shadow"
    },
    1u
  );    
  std::vector< std::shared_ptr< gct::mappable_buffer_t > > dynamic_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > dynamic_descriptor_set;
  for( std::size_t i = 0u; i != res.swapchain_images.size(); ++i ) {
    dynamic_uniform.emplace_back(
      res.allocator->create_mappable_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eUniformBuffer
      )
    );
    dynamic_descriptor_set.push_back(
      res.descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )   
    );    
    dynamic_descriptor_set.back()->update({
      { "dynamic_uniforms", dynamic_uniform.back() },
      { "shadow", cubemap_sampler, cubemap_images.get_cube_image_views()[ i ], vk::ImageLayout::eShaderReadOnlyOptimal }
    }); 
  }

  const auto r32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR32Sfloat )
          .setUsage( vk::ImageUsageFlagBits::eStorage )
      );
  
  const auto diffuse = res.allocator->create_image_views(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );
  const auto specular = res.allocator->create_image_views(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( diffuse, vk::ImageLayout::eGeneral );
      recorder.set_image_layout( specular, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  const auto lighting = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/lighting/lighting.comp.spv" )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "diffuse_image", diffuse } )
      .add_resource( { "specular_image", specular } )
      .add_resource( { "dynamic_uniforms", dynamic_uniform } )
  );

  gct::hbao hbao(
    gct::hbao_create_info()
      .set_allocator( res.allocator )
      .set_pipeline_cache( res.pipeline_cache )
      .set_descriptor_pool( res.descriptor_pool )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/ao/" )
      .set_input_name( "gbuffer" )
      .set_input( gbuffer.get_image_views() )
      .add_resource( { "dynamic_uniforms", dynamic_uniform } ) 
  );

  for( std::size_t i = 0u; i != res.swapchain_images.size(); ++i ) {
    framebuffers.emplace_back(
      fb_resources_t{
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.device->get_semaphore(),
        res.queue->get_command_pool()->allocate(),
        gbuffer.get_render_pass_begin_info( i )
      }
    );
  }

  const auto mixed_out = res.allocator->create_image_views(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto recorder = command_buffer->begin();
      recorder.set_image_layout( mixed_out, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }

  const gct::tone_mapping tone(
    gct::tone_mapping_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/tone/tone.comp.spv" )
      .set_input( mixed_out )
  );

  auto mix_ao = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/mix_ao/mix_ao.comp.spv" )
      .set_input_name( "diffuse_image" )
      .set_input( diffuse )
      .set_output_name( "bloom_image" )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } ) 
      .add_resource( { "specular_image", specular } ) 
      .add_resource( { "occlusion", hbao.get_output(), vk::ImageLayout::eGeneral } )
      .add_resource( { "skyview", linear_sampler, skyview, vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "scattering", rendered_froxel, vk::ImageLayout::eGeneral } )
      .add_resource( { "dest_image", mixed_out } ) 
      .add_resource( { "tone", tone.get_buffer() } ) 
      .add_resource( { "dynamic_uniforms", dynamic_uniform } ) 
  );

  const auto bloom_gauss = mix_ao( 
    gct::image_filter_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gauss/h12_32.comp.spv" )
  )(
    gct::image_filter_create_info()
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gauss/v12_32.comp.spv" )
  );

  const auto gamma = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv" )
      .set_input( mixed_out )
      .set_output( res.swapchain_image_views )
      .add_resource( { "bloom_image", bloom_gauss.get_output(), vk::ImageLayout::eGeneral } )
  );
  
  const auto capture = res.allocator->create_image_views(
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
          .setFormat( vk::Format::eR8G8B8A8Unorm )
          .setUsage( vk::ImageUsageFlagBits::eStorage|vk::ImageUsageFlagBits::eTransferSrc )
      ),
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );

  const auto gamma2 = gct::image_filter(
    gct::image_filter_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/gamma/gamma.comp.spv" )
      .set_input( mixed_out )
      .set_output( capture )
      .add_resource( { "bloom_image", bloom_gauss.get_output(), vk::ImageLayout::eGeneral } )
  );
  

  gct::gltf::document_t doc;
  {
    const auto gcb = res.queue->get_command_pool()->allocate();
    {
      auto rec = gcb->begin();
      doc = gct::gltf::load_gltf(
        res.model_filename,
        res.device,
        rec,
        res.allocator,
        res.descriptor_pool,
        {
          shadow_gbuffer.get_render_pass(),
          gbuffer.get_render_pass()
        },
        {
          CMAKE_CURRENT_BINARY_DIR "/shadow",
          CMAKE_CURRENT_BINARY_DIR "/geometry"
        },
        0,
        framebuffers.size(),
        0,
        float( res.width ) / float( res.height ),
        false,
        {
          dynamic_descriptor_set_layout
        }
      );
    }
    gcb->execute_and_wait();
  }

  auto center = ( doc.node.min + doc.node.max ) / 2.f;
  auto scale = std::abs( glm::length( doc.node.max - doc.node.min ) );

  const glm::mat4 projection = glm::perspective( 0.6981317007977318f, (float(res.width)/float(res.height)), std::min(0.1f*scale,0.5f), scale );
  const float light_size = 0.3;

  gct::glfw_walk walk( center, scale, res.walk_state_filename );
  const auto point_lights = gct::gltf::get_point_lights(
    doc.node,
    doc.point_light
  );
  if( !point_lights.empty() ) {
    walk.set_light_energy( point_lights[ 0 ].intensity / ( 4 * M_PI ) / 100 );
    walk.set_light_pos( point_lights[ 0 ].location );
  }
  res.window->set_on_key(
    [&walk]( gct::glfw_window &p, int key, int scancode, int action, int mods ) {
      walk( p, key, scancode, action, mods );
    }
  );
  res.window->set_on_closed(
    [&walk]( auto & ) {
      walk.set_end();
    }
  );

  std::vector< std::shared_ptr< gct::mappable_buffer_t > > shadow_uniform;
  std::vector< std::shared_ptr< gct::descriptor_set_t > > shadow_descriptor_set;
  for( std::size_t i = 0u; i != res.swapchain_images.size(); ++i ) {
    shadow_uniform.emplace_back(
      res.allocator->create_mappable_buffer(
        sizeof( gct::gltf::dynamic_uniforms_t ),
        vk::BufferUsageFlagBits::eUniformBuffer
      )
    );
    shadow_descriptor_set.push_back(
      res.descriptor_pool->allocate(
        dynamic_descriptor_set_layout
      )
    );
    shadow_descriptor_set.back()->update({
      { "dynamic_uniforms", shadow_uniform.back() },
      { "matrices", shadow_mat.get_buffer()[ i ] }
    });
  }
  
  uint32_t current_frame = 0u;
  uint32_t last_image_index = framebuffers.size();
  std::uint32_t frame_counter = 0u;
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;

    auto &sync = framebuffers[ current_frame ];
    if( !sync.initial ) {
      sync.command_buffer->wait_for_executed();
    }
    else sync.initial = false;
    auto image_index = res.swapchain->acquire_next_image( sync.image_acquired );
    auto &fb = framebuffers[ image_index ];

    {
      auto rec = sync.command_buffer->begin();

      tone.set( rec, image_index );

      rec.convert_image(
        skyview[ 0 ]->get_factory(),
        vk::ImageLayout::eGeneral
      );
      {
        auto current_camera_pos = walk.get_camera_pos();
        current_camera_pos.y -= 100;
        walk.set_camera_pos( current_camera_pos );
        float altitude = 6360000.f + frame_counter * 100;
        ++walk;
        auto params = skyview_parameters_t()
          .set_sigma_ma( sigma_ma )
          .set_sigma_oa( sigma_oa )
          .set_sigma_rs( sigma_rs )
          .set_sigma_ms( sigma_ms )
          .set_light_pos( light_pos )
          .set_ground_radius( ground_radius )
          .set_top_radius( top_radius )
          .set_g( 0.8f )
          .set_altitude( altitude )
          .set_light_energy( light_energy );
        rec->pushConstants(
          **generate_skyview.get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( skyview_parameters_t ),
          reinterpret_cast< void* >( &params )
        );
      }
      generate_skyview( rec, 0u, skyview_width, skyview_height, 1u );
      rec.convert_image(
        skyview[ 0 ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );

      shadow_mat( rec, light_pos_near, 180.f, 200.f, image_index );
      
      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( light_pos_near )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( res.ambient_level );
        rec.copy(
          dynamic_data,
          shadow_uniform[ image_index ]
        );
      }

      rec.transfer_to_graphics_barrier(
        {
          shadow_uniform[ image_index ]->get_buffer()
        },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          shadow_gbuffer.get_render_pass_begin_info( image_index ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &shadow_gbuffer.get_viewport() );
        rec->setScissor( 0, 1, &shadow_gbuffer.get_scissor() );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          0u,
          {
            shadow_descriptor_set[ image_index ]
          }
        );
      }
      rec.convert_image(
        shadow_gbuffer.get_image( image_index ),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      rec.barrier(
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eFragmentShader,
        vk::DependencyFlagBits( 0 ),
        {},
        {
          shadow_gbuffer.get_image( image_index )
        }
      );

      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_projection_matrix( projection )
          .set_camera_matrix( walk.get_lookat() )
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( light_pos_near )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( res.ambient_level )
          .set_inversed_voxel( glm::inverse( projection * walk.get_lookat() ) );
        rec.copy(
          dynamic_data,
          dynamic_uniform[ image_index ]
        );
      }
      rec.transfer_to_graphics_barrier(
        { dynamic_uniform[ image_index ]->get_buffer() },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          fb.render_pass_begin_info,
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &gbuffer.get_viewport() );
        rec->setScissor( 0, 1, &gbuffer.get_scissor() );
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
    
      lighting( rec, image_index, res.width, res.height, 1u );
      
      rec.compute_barrier(
        {},
        { diffuse[ image_index ]->get_factory() }
      );

      hbao( rec, image_index );

      rec.compute_barrier(
        {},
        {
          hbao.get_output()[ image_index ]->get_factory(),
          diffuse[ image_index ]->get_factory(),
          specular[ image_index ]->get_factory()
        }
      );
      
      {
        const glm::mat4 world_to_screen = projection * walk.get_lookat();
        const glm::mat4 screen_to_world = glm::inverse( world_to_screen );
        auto fs = froxel_state_t()
          .set_world_to_screen( world_to_screen )
          .set_screen_to_world( screen_to_world )
          .set_sigma_ma( sigma_ma )
          .set_sigma_oa( sigma_oa )
          .set_sigma_rs( sigma_rs )
          .set_sigma_ms( sigma_ms )
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( light_pos_near )
          .set_ground_radius( ground_radius )
          .set_top_radius( top_radius )
          .set_g( 0.8f )
          .set_altitude( 6360100.f )
          .set_light_energy( light_energy );
        rec.copy(
          fs,
          froxel_state[ image_index ]
        );
      }

      rec.convert_image(
        froxel[ image_index ]->get_factory(),
        vk::ImageLayout::eGeneral
      );
      
      calc_scattering( rec, image_index, froxel_xy_resolution, froxel_xy_resolution, froxel_z_resolution );

      rec.convert_image(
        froxel[ image_index ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      
      render_scattering( rec, image_index, res.width, res.height, 1u );
      
      rec.compute_barrier(
        {},
        {
          rendered_froxel[ image_index ]->get_factory()
        }
      );
      
      bloom_gauss( rec, image_index );
     
      tone.get( rec, image_index );

      gamma( rec, image_index );
      rec.convert_image(
        capture[ image_index ]->get_factory(),
        vk::ImageLayout::eGeneral
      );
      gamma2( rec, image_index );

      rec.convert_image(
        res.swapchain_images[ image_index ],
        vk::ImageLayout::ePresentSrcKHR
      );
      /*rec.convert_image(
        capture[ image_index ]->get_factory(),
        vk::ImageLayout::eTransferSrcOptimal
      );
      rec.dump_image(
        res.allocator,
        capture[ image_index ]->get_factory(),
        std::string( "sky_" ) + std::to_string( frame_counter ) + ".png",
        0
      );*/
    }
    sync.command_buffer->execute(
      gct::submit_info_t()
        .add_wait_for( sync.image_acquired, vk::PipelineStageFlagBits::eColorAttachmentOutput )
        .add_signal_to( sync.draw_complete )
    );
    res.queue->present(
      gct::present_info_t()
        .add_wait_for( sync.draw_complete )
        .add_swapchain( res.swapchain, image_index )
    );
    last_image_index = image_index;
    glfwPollEvents();
    ++current_frame;
    current_frame %= framebuffers.size();
    ++frame_counter;
  }
  (*res.queue)->waitIdle();
  walk.save( res.walk_state_filename );
}

