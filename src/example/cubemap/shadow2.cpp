  const auto [shadow_mat_descriptor_set_layout,shadow_mat_pipeline] = pipeline_cache->get_pipeline(
    CMAKE_CURRENT_BINARY_DIR "/shadow_mat/shadow_mat.comp.spv"
  );
  const auto shadow_mat_storage = allocator->create_buffer(
    sizeof( shadow_matrices_t ),
    vk::BufferUsageFlagBits::eStorageBuffer,
    VMA_MEMORY_USAGE_GPU_ONLY
  );
  const auto shadow_mat_descriptor_set = descriptor_pool->allocate(
    shadow_mat_descriptor_set_layout
  );
  shadow_mat_descriptor_set->update({
    { "matrices", shadow_mat_storage }
  });
  const auto shadow_uniform = allocator->create_mappable_buffer(
    sizeof( gct::gltf::dynamic_uniforms_t ),
    vk::BufferUsageFlagBits::eUniformBuffer
  );
  const auto shadow_descriptor_set = descriptor_pool->allocate(
    dynamic_descriptor_set_layout
  );
  shadow_descriptor_set->update({
    { "dynamic_uniforms", shadow_uniform },
    { "matrices", shadow_mat_storage }
  });
  const unsigned int shadow_map_size = 1024u;
  gct::gbuffer shadow_gbuffer(
    gct::gbuffer_create_info()
      .set_allocator( allocator )
      .set_width( shadow_map_size )
      .set_height( shadow_map_size )
      .set_layer( 6 )
      .set_swapchain_image_count( 1 )
      .set_color_buffer_count( 1 )
      .set_flags( vk::ImageCreateFlagBits::eCubeCompatible )
      .set_format( vk::Format::eR32G32Sfloat ) 
      .set_final_layout( vk::ImageLayout::eColorAttachmentOptimal )
      .set_clear_color( gct::color::web::white )
  );

