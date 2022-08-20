add_executable( glsl_include glsl_include.cpp )
target_link_libraries( glsl_include
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

add_library( gct SHARED
  gct/get_instance_version.cpp
  gct/get_instance_layers.cpp
  gct/get_instance_extensions.cpp
  gct/get_device_layers.cpp
  gct/get_device_extensions.cpp
  gct/to_json.cpp
  gct/queue_family_properties.cpp
  gct/physical_device_properties.cpp
  gct/physical_device_memory_properties.cpp
  gct/physical_device_features.cpp
  gct/physical_device.cpp
  gct/instance_create_info.cpp
  gct/instance.cpp
  gct/surface.cpp
  gct/surface_capabilities.cpp
  gct/queue.cpp
  gct/device_create_info.cpp
  gct/device.cpp
  gct/command_pool.cpp
  gct/allocator.cpp
  gct/image_create_info.cpp
  gct/image.cpp
  gct/astc.cpp
  gct/load_astc.cpp
  gct/mmaped_file.cpp
  gct/round_up.cpp
  gct/acceleration_structure_create_info.cpp
  gct/acceleration_structure.cpp
  gct/acceleration_structure_geometry_triangles_data.cpp
  gct/acceleration_structure_geometry_aabbs_data.cpp
  gct/acceleration_structure_geometry_instances_data.cpp
  gct/acceleration_structure_geometry.cpp
  gct/acceleration_structure_build_sizes_info.cpp
  gct/acceleration_structure_build_geometry_info.cpp
  gct/acceleration_structure_device_address_info.cpp
  gct/allocated_image.cpp
  gct/swapchain_image.cpp
  gct/buffer_create_info.cpp
  gct/buffer.cpp
  gct/buffer_device_address_info.cpp
  gct/extension.cpp
  gct/swapchain_create_info.cpp
  gct/swapchain.cpp
  gct/descriptor_pool_create_info.cpp
  gct/descriptor_pool.cpp
  gct/descriptor_set_layout_create_info.cpp
  gct/descriptor_set_layout.cpp
  gct/descriptor_set_allocate_info.cpp
  gct/descriptor_set.cpp
  gct/specialized_info.cpp
  gct/pipeline_cache_create_info.cpp
  gct/pipeline_cache.cpp
  gct/pipeline_layout_create_info.cpp
  gct/pipeline_layout.cpp
  gct/pipeline_vertex_input_state_create_info.cpp
  gct/pipeline_shader_stage_create_info.cpp
  gct/pipeline_input_assembly_state_create_info.cpp
  gct/pipeline_tessellation_state_create_info.cpp
  gct/pipeline_rasterization_state_create_info.cpp 
  gct/pipeline_multisample_state_create_info.cpp 
  gct/pipeline_depth_stencil_state_create_info.cpp 
  gct/pipeline_viewport_state_create_info.cpp
  gct/pipeline_color_blend_state_create_info.cpp 
  gct/pipeline_dynamic_state_create_info.cpp
  gct/compute_pipeline_create_info.cpp
  gct/compute_pipeline.cpp
  gct/graphics_pipeline_create_info.cpp
  gct/graphics_pipeline.cpp
  gct/descriptor_image_info.cpp
  gct/descriptor_buffer_info.cpp
  gct/write_descriptor_set.cpp
  gct/subpass_description.cpp
  gct/render_pass_create_info.cpp
  gct/render_pass_begin_info.cpp
  gct/render_pass.cpp
  gct/begin_render_pass.cpp
  gct/image_view_create_info.cpp
  gct/image_view.cpp
  gct/framebuffer_create_info.cpp
  gct/framebuffer.cpp
  gct/buffer_view_create_info.cpp
  gct/buffer_view.cpp
  gct/shader_module_create_info.cpp
  gct/shader_module.cpp
  gct/sampler_create_info.cpp
  gct/sampler.cpp
  gct/semaphore_create_info.cpp
  gct/semaphore.cpp
  gct/fence_create_info.cpp
  gct/fence.cpp
  gct/command_buffer_allocate_info.cpp
  gct/command_buffer_begin_info.cpp
  gct/command_buffer.cpp
  gct/command_buffer_recorder.cpp
  gct/load_buffer.cpp
  gct/load_image.cpp
  gct/copy_buffer.cpp
  gct/copy_image.cpp
  gct/copy_buffer_to_image.cpp
  gct/copy_image_to_buffer.cpp
  gct/blit.cpp
  gct/barrier.cpp
  gct/build_acceleration_structure.cpp
  gct/present_info.cpp
  gct/wait_for_sync.cpp
  gct/bind_descriptor_set.cpp
  gct/bind_pipeline.cpp
  gct/bind_vertex_buffer.cpp
  gct/format_to_aspect.cpp
  gct/format_to_size.cpp
  gct/format_to_type.cpp
  gct/format_to_channels.cpp
  gct/get_all_formats.cpp
  gct/pixel_per_element_x.cpp
  gct/pixel_per_element_y.cpp
  gct/is_rgba.cpp
  gct/is_bgra.cpp
  gct/submit_info.cpp
  gct/gltf.cpp
  gct/shader_flag.cpp
  gct/load_gltf.cpp
  gct/load_mesh.cpp
  gct/load_node.cpp
  gct/generate_triangle.cpp
  gct/generate_blas.cpp
  gct/generate_tlas.cpp
  gct/vma.cpp
  gct/dispatch_loader.cpp
  gct/vulkanhpp.cpp
  gct/device_address.cpp
  gct/device_or_host_address_const.cpp
  gct/device_or_host_address.cpp
  gct/strided_device_address_region.cpp
  gct/deferred_operation.cpp
  gct/pipeline_library_create_info.cpp
  gct/ray_tracing_shader_group_create_info.cpp
  gct/ray_tracing_pipeline_interface_create_info.cpp
  gct/ray_tracing_pipeline_create_info.cpp
  gct/ray_tracing_pipeline.cpp
  gct/trace_rays.cpp
  gct/mp4.cpp
  gct/io_context.cpp
  gct/async.cpp
  gct/spv2json.cpp
  gct/spv2vk.cpp
  gct/image_layout.cpp
  gct/numeric_types.cpp
  gct/get_normalized_vertex_attribute_name.cpp
  gct/vertex_attribute_name_to_usage.cpp
  gct/get_vertex_attributes.cpp
  gct/is_srgb.cpp
  gct/is_normalized.cpp
  gct/is_scaled.cpp
  gct/is_signed.cpp
  gct/format_to_component_size.cpp
  gct/format_to_component_type.cpp
  gct/format2numeric_type.cpp
  gct/vertex_attribute.cpp
  gct/primitives/triangle.cpp
  gct/primitives/cube.cpp
  gct/primitives/sphere.cpp
  ../external/SPIRV-Reflect/spirv_reflect.c
)

target_link_libraries(
  gct
  Threads::Threads
  ${CMAKE_DL_LIBS}
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_VULKAN2JSON_vulkan2json}
)

add_library( gct_glfw SHARED
  gct/glfw.cpp
)

set_target_properties(
  gct
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

install( TARGETS gct )

target_link_libraries(
  gct_glfw
  Threads::Threads
  ${CMAKE_DL_LIBS}
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

set_target_properties(
  gct_glfw
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

install( TARGETS gct_glfw )

add_executable( graphics
  example/graphics.cpp
)

target_link_libraries(
  graphics
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::context
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_executable( compute
  example/compute.cpp
)

target_link_libraries(
  compute
  gct
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
)

add_executable( test
  example/test.cpp
)

target_link_libraries(
  test
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_executable( test2
  example/test2.cpp
)

target_link_libraries(
  test2
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)


add_executable( image
  example/image.cpp
)

target_link_libraries(
  image
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

add_executable( ball
  example/ball.cpp
)

target_link_libraries(
  ball
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_GLFW_glfw}
  ${pkgcfg_lib_GLFW_glfw3}
)

subdirs( example )
