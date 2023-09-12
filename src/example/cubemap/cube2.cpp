  const auto [cube_mat_descriptor_set_layout,cube_mat_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/cubemat/cubemat.comp.spv"
  );
  const auto cube_mat_storage = allocator->create_buffer(
    sizeof( cube_matrices_t ),
    vk::BufferUsageFlagBits::eStorageBuffer,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto cube_mat_descriptor_set = descriptor_pool->allocate(
    cube_mat_descriptor_set_layout
  );
  cube_mat_descriptor_set->update({
    { "matrices", cube_mat_storage }
  });
  const auto cube_uniform = allocator->create_mappable_buffer(
    sizeof( gct::gltf::dynamic_uniforms_t ),
    vk::BufferUsageFlagBits::eUniformBuffer
  );
  const auto cube_descriptor_set = descriptor_pool->allocate(
    dynamic_descriptor_set_layout
  );
  cube_descriptor_set->update({
    { "dynamic_uniforms", cube_uniform },
    { "matrices", cube_mat_storage },
    { "shadow", cubemap_sampler, shadow_cubemap_images.get_cube_image_views()[ 0 ], vk::ImageLayout::eShaderReadOnlyOptimal }
  });
  const unsigned int sh_grid_size = 128u;
  const unsigned int sh_grid_block_size = 1u;
  const unsigned int sh_layer_count = 6u * sh_grid_block_size * sh_grid_block_size * sh_grid_block_size;
  const unsigned int cube_gbuffer_attrs = 8u;
  const unsigned int cube_gbuffer_image_size = 320u;
  gct::gbuffer cube_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( allocator )
      .set_width( cube_gbuffer_image_size )
      .set_height( cube_gbuffer_image_size )
      .set_layer( sh_layer_count )
      .set_swapchain_image_count( 1 )
      .set_color_buffer_count( cube_gbuffer_attrs )
      .set_format( vk::Format::eR32G32B32A32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
  );


