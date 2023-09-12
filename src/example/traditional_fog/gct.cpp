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
#include <gct/common_sample_setup.hpp>
#include <gct/tone_mapping.hpp>
#include <gct/hbao.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  gct::render_pass_begin_info_t render_pass_begin_info;
  bool initial = true;
};

struct traditional_fog_parameters_t {
  LIBGCT_SETTER( fog_color )
  LIBGCT_SETTER( fog_density )
  glm::vec4 fog_color;
  float fog_density;
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
  const auto rgba32ici =
    gct::image_create_info_t()
      .set_basic(
        gct::basic_2d_image( res.width, res.height )
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
  while( !walk.end() ) {
    gct::blocking_timer frame_rate;
    ++walk;

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

      shadow_mat( rec, walk.get_light_pos(), std::min( 0.1f*scale, 0.5f ), 2.f*scale, image_index );

      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
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
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( res.ambient_level );
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
        auto params = traditional_fog_parameters_t()
          .set_fog_color( glm::vec4( 0.8, 0.8, 0.8, 1.0f ) )
          .set_fog_density( 0.03 );
        rec->pushConstants(
          **mix_ao.get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( traditional_fog_parameters_t ),
          reinterpret_cast< void* >( &params )
        );
      }


      bloom_gauss( rec, image_index );
     
      tone.get( rec, image_index );

      gamma( rec, image_index );

      rec.convert_image(
        res.swapchain_images[ image_index ],
        vk::ImageLayout::ePresentSrcKHR
      );
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
  }
  (*res.queue)->waitIdle();
  walk.save( res.walk_state_filename );
}

