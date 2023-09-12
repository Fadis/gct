      {
        auto cube_mat_pc = gct::cubemat_push_constant_t()
          .set_ioffset( glm::ivec4( 0, 0, 0, 0 ) )
          .set_offset( glm::vec4( walk.get_light_pos(), 1.f ) )
          .set_near( std::min( 0.1f*scale, 0.5f ) )
          .set_far( 2.f*scale );
        rec->pushConstants(
          **shadow_mat_pipeline->get_props().get_layout(),
          vk::ShaderStageFlagBits::eCompute,
          0u,
          sizeof( gct::cubemat_push_constant_t ),
          reinterpret_cast< void* >( &cube_mat_pc )
        );
      }
      rec.bind(
        shadow_mat_pipeline,
        { shadow_mat_descriptor_set }
      );
      rec.dispatch_threads( 6, 1, 1 );
      rec.barrier(
        vk::AccessFlagBits::eShaderWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eComputeShader,
        vk::PipelineStageFlagBits::eGeometryShader,
        vk::DependencyFlagBits( 0 ),
        {
          shadow_mat_storage
        },
        {}
      );
 
      {
        auto dynamic_data = gct::gltf::dynamic_uniforms_t()
          .set_eye_pos( glm::vec4( walk.get_light_pos(), 1.f ) )
          .set_light_pos( glm::vec4( walk.get_light_pos(), 1.0 ) )
          .set_light_energy( walk.get_light_energy() )
          .set_light_size( light_size )
          .set_ambient( ambient_level );
        rec.copy(
          dynamic_data,
          shadow_uniform
        );
      }
 
      rec.barrier(
        vk::AccessFlagBits::eTransferWrite,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eTransfer,
        vk::PipelineStageFlagBits::eVertexShader,
        vk::DependencyFlagBits( 0 ),
        {
          shadow_uniform->get_buffer()
        },
        {}
      );
      {
        auto render_pass_token = rec.begin_render_pass(
          shadow_gbuffer.get_render_pass_begin_info( 0 ),
          vk::SubpassContents::eInline
        );
        rec->setViewport( 0, 1, &shadow_viewport );
        rec->setScissor( 0, 1, &shadow_scissor );
        gct::gltf::draw_node(
          rec,
          doc.node,
          doc.mesh,
          doc.buffer,
          0u,
          {
            shadow_descriptor_set
          }
        );
      }
      rec.convert_image(
        shadow_gbuffer.get_image( 0 ),
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
          shadow_gbuffer.get_image( 0 )
        }
      );


