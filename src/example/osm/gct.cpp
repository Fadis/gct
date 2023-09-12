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
#include <gct/graphics.hpp>
#include <gct/graphics_pipeline.hpp>
#include <gct/common_sample_setup.hpp>
#include <gct/tone_mapping.hpp>
#include <gct/hbao.hpp>
#include <gct/draw_particle.hpp>
#include <gct/particle_buffer.hpp>
#include <gct/color.hpp>
#include <gct/generate_projection_matrix.hpp>

struct fb_resources_t {
  std::shared_ptr< gct::semaphore_t > image_acquired;
  std::shared_ptr< gct::semaphore_t > draw_complete;
  std::shared_ptr< gct::semaphore_t > image_ownership;
  std::shared_ptr< gct::bound_command_buffer_t > command_buffer;
  gct::render_pass_begin_info_t render_pass_begin_info;
  bool initial = true;
};

struct particle_extinction_parameters {
  LIBGCT_SETTER( near )
  LIBGCT_SETTER( far )
  LIBGCT_SETTER( aspect )
  LIBGCT_SETTER( point_size )
  LIBGCT_SETTER( split_count )
  float near;
  float far;
  float aspect;
  float point_size;
  std::uint32_t split_count;
};

struct scattering_parameters {
  LIBGCT_SETTER( g )
  LIBGCT_SETTER( color )
  LIBGCT_SETTER( eye_pos )
  LIBGCT_SETTER( light_pos )
  LIBGCT_SETTER( light_energy )
  LIBGCT_SETTER( light_near )
  LIBGCT_SETTER( light_far )
  LIBGCT_SETTER( view_near )
  LIBGCT_SETTER( view_far )
  LIBGCT_SETTER( split_count )
  glm::vec4 g;
  glm::vec4 color;
  glm::vec4 eye_pos;
  glm::vec4 light_pos;
  glm::vec4 light_energy;
  float light_near;
  float light_far;
  float view_near;
  float view_far;
  std::uint32_t split_count;
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
  
  auto linear_sampler = res.device->get_sampler(
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
      { "shadow", linear_sampler, cubemap_images.get_cube_image_views()[ i ], vk::ImageLayout::eShaderReadOnlyOptimal }
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
 
  constexpr static std::uint32_t particle_shadow_map_size = 512u;
  std::shared_ptr< gct::gbuffer > particle_light_extinction_gbuffer( new gct::gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( particle_shadow_map_size )
      .set_height( particle_shadow_map_size )
      .set_layer( 6 * 32 )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .set_color_buffer_count( 1 )
      .set_format( vk::Format::eR32Sfloat )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::special::transparent )
      .disable_depth()
  ) );
  
  gct::cubemap_images2 particle_light_extinction_cubemap_images( particle_light_extinction_gbuffer->get_image_views() );
  
  std::shared_ptr< gct::gbuffer > particle_view_extinction_gbuffer( new gct::gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( res.allocator )
      .set_width( res.width )
      .set_height( res.height )
      .set_layer( 32 )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .set_color_buffer_count( 1 )
      .set_format( vk::Format::eR32Sfloat )
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::special::transparent )
      .disable_depth()
  ) );
  
  gct::draw_particle draw_particle_light_extinction(
    gct::draw_particle_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_output( particle_light_extinction_gbuffer )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/particle_shadow/light_extinction.vert.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/particle_shadow/light_extinction.geom.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/particle_shadow/light_extinction.frag.spv" )
      .add_resource( { "dynamic_uniforms", dynamic_uniform } )
      .add_resource( { "matrices", shadow_mat.get_buffer() } )
  );
  
  gct::particle_buffer particle_buffer(
    gct::particle_buffer_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_filename( CMAKE_CURRENT_SOURCE_DIR "/smoke.obj" )
      .set_vamap( draw_particle_light_extinction.get_vamap() )
      .set_stride( draw_particle_light_extinction.get_stride() )
  );

  
  gct::draw_particle draw_particle_view_extinction(
    gct::draw_particle_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_output( particle_view_extinction_gbuffer )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/particle_shadow/view_extinction.vert.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/particle_shadow/view_extinction.geom.spv" )
      .add_shader( CMAKE_CURRENT_BINARY_DIR "/particle_shadow/view_extinction.frag.spv" )
      .add_resource( { "dynamic_uniforms", dynamic_uniform } )
  );

  const auto rendered_scattering = res.allocator->create_image_views(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );
  const auto rendered_extinction = res.allocator->create_image_views(
    rgba32ici,
    VMA_MEMORY_USAGE_GPU_ONLY,
    res.swapchain_images.size()
  );

  {
    auto command_buffer = res.queue->get_command_pool()->allocate();
    {
      auto rec = command_buffer->begin();
      particle_buffer.init( rec );
      rec.set_image_layout( rendered_scattering, vk::ImageLayout::eGeneral );
      rec.set_image_layout( rendered_extinction, vk::ImageLayout::eGeneral );
    }
    command_buffer->execute_and_wait();
  }
  
  const auto render_scattering = gct::compute(
    gct::compute_create_info()
      .set_allocator( res.allocator )
      .set_descriptor_pool( res.descriptor_pool )
      .set_pipeline_cache( res.pipeline_cache )
      .set_shader( CMAKE_CURRENT_BINARY_DIR "/volumetric_light/render_scattering.comp.spv" )
      .set_swapchain_image_count( res.swapchain_images.size() )
      .add_resource( { "gbuffer", gbuffer.get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "light_extinction", linear_sampler, particle_light_extinction_cubemap_images.get_cube_image_views(), vk::ImageLayout::eShaderReadOnlyOptimal } )
      .add_resource( { "view_extinction", particle_view_extinction_gbuffer->get_image_views(), vk::ImageLayout::eGeneral } )
      .add_resource( { "scattering", rendered_scattering, vk::ImageLayout::eGeneral } )
      .add_resource( { "extinction", rendered_extinction, vk::ImageLayout::eGeneral } )
      .add_resource( { "shadow", linear_sampler, cubemap_images.get_cube_image_views(), vk::ImageLayout::eShaderReadOnlyOptimal } )
  );

  const auto bloom_gauss = gct::image_filter(
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
      .add_resource( { "scattering", rendered_scattering } )
      .add_resource( { "extinction", rendered_extinction } )
      .add_resource( { "dest_image", mixed_out } ) 
      .add_resource( { "tone", tone.get_buffer() } ) 
      .add_resource( { "dynamic_uniforms", dynamic_uniform } ) 
  )( 
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
  std::cout << "old near : " << std::min(0.1f*scale,0.5f) << "\told far : " << scale << std::endl;
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
  uint32_t frame_counter = 0u;
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

      const auto [shadow_near,shadow_far] = gct::generate_cube_projection_distance(
        walk.get_light_pos(),
        {
          { doc.node.min, doc.node.max },
          particle_buffer.get_aabb()
        }
      );
      std::cout << "shadow near : " << shadow_near << "\tshadow far : " << shadow_far << std::endl;

      shadow_mat( rec, walk.get_light_pos(), shadow_near, shadow_far, image_index );

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

      const auto [projection,near,far] = gct::generate_projection_matrix(
        walk.get_lookat(),
        0.6981317007977318f,
        res.width,
        res.height,
        {
          { doc.node.min, doc.node.max },
          particle_buffer.get_aabb()
        }
      );
      std::cout << "near : " << shadow_near << "\tfar : " << shadow_far << std::endl;
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
      
      rec.barrier(
        vk::AccessFlagBits::eColorAttachmentWrite,
        vk::AccessFlagBits::eColorAttachmentRead,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::PipelineStageFlagBits::eColorAttachmentOutput,
        vk::DependencyFlagBits( 0 ),
        {},
        {
          shadow_gbuffer.get_depth_views()[ image_index ]->get_factory(),
          gbuffer.get_depth_views()[ image_index ]->get_factory()
        }
      );

      rec.convert_image(
        particle_light_extinction_gbuffer->get_image_views()[ image_index ]->get_factory(),
        vk::ImageLayout::eGeneral
      );
      
      {
        auto params = particle_extinction_parameters()
          .set_near( near )
          .set_far( far )
          .set_aspect( float( res.height )/float( res.width ) )
          .set_point_size( 2.0 )
          .set_split_count( 32 );
        rec->pushConstants(
          **draw_particle_light_extinction.get_graphics()->get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eGeometry,
          0u,
          sizeof( particle_extinction_parameters ),
          reinterpret_cast< void* >( &params )
        );
      }

      draw_particle_light_extinction( rec, image_index, particle_buffer );
      
      {
        auto params = particle_extinction_parameters()
          .set_near( near )
          .set_far( far )
          .set_aspect( float( res.height )/float( res.width ) )
          .set_point_size( 2.0 )
          .set_split_count( 32 );
        rec->pushConstants(
          **draw_particle_view_extinction.get_graphics()->get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eGeometry,
          0u,
          sizeof( particle_extinction_parameters ),
          reinterpret_cast< void* >( &params )
        );
      }

      draw_particle_view_extinction( rec, image_index, particle_buffer );
      
      rec.convert_image(
        particle_light_extinction_gbuffer->get_image_views()[ image_index ]->get_factory(),
        vk::ImageLayout::eShaderReadOnlyOptimal
      );
      
      rec.convert_image(
        particle_view_extinction_gbuffer->get_image_views()[ image_index ]->get_factory(),
        vk::ImageLayout::eGeneral
      );

      {
        auto params = scattering_parameters()
          .set_g( glm::vec4( 0.3, 0.0, 0.0, 1.0 ) )
          .set_color( glm::vec4( 0.25, 0.25, 0.25, 1.0 ) )
          .set_eye_pos( glm::vec4( walk.get_camera_pos(), 1.0 ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( glm::vec4( walk.get_light_energy(), walk.get_light_energy(), walk.get_light_energy(), 1.0 ) )
          .set_light_near( shadow_near )
          .set_light_far( shadow_far )
          .set_view_near( near )
          .set_view_far( far )
          .set_split_count( 32u );
        rec->pushConstants(
          **render_scattering.get_pipeline()->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( scattering_parameters ),
          reinterpret_cast< void* >( &params )
        );
      }

      render_scattering( rec, image_index, res.width, res.height, 1u );

      rec.compute_barrier(
        {},
        {
          rendered_scattering[ image_index ]->get_factory(),
          rendered_extinction[ image_index ]->get_factory()
        }
      );

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
    ++frame_counter;
    current_frame %= framebuffers.size();
  }
  (*res.queue)->waitIdle();
  walk.save( res.walk_state_filename );
}

