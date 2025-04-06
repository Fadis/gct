add_executable( glsl_include glsl_include.cpp )
target_link_libraries( glsl_include
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

add_executable(
  ttf_to_gltf
  ttf_to_gltf.cpp
  ../external/ttf2mesh/ttf2mesh.c
)
target_link_libraries( ttf_to_gltf
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)
target_link_libraries( ttf_to_gltf
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

add_library( gct SHARED
  gct/get_instance_version.cpp
  gct/get_instance_layers.cpp
  gct/get_instance_extensions.cpp
  gct/get_device_layers.cpp
  gct/get_device_extensions.cpp
  gct/to_json.cpp
  gct/to_matrix.cpp
  gct/queue_family_properties.cpp
  gct/physical_device_properties.cpp
  gct/physical_device_memory_properties.cpp
  gct/physical_device_features.cpp
  gct/format_properties.cpp
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
  gct/allocator_set.cpp
  gct/syncable.cpp
  gct/image_shrink_info.cpp
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
  gct/mappable_buffer.cpp
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
  gct/specialized_map.cpp
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
  gct/attachment_reference2.cpp
  gct/attachment_description2.cpp
  gct/subpass_description2.cpp
  gct/subpass_dependency2.cpp
  gct/render_pass_create_info2.cpp
  gct/render_pass_begin_info.cpp
  gct/render_pass.cpp
  gct/render_pass2.cpp
  gct/rendering_info.cpp
  gct/begin_render_pass.cpp
  gct/begin_rendering.cpp
  gct/begin_query.cpp
  gct/conditional_rendering.cpp
  gct/image_view_create_info.cpp
  gct/image_view.cpp
  gct/framebuffer_create_info.cpp
  gct/framebuffer.cpp
  gct/buffer_view_create_info.cpp
  gct/buffer_view.cpp
  gct/shader_module_create_info.cpp
  gct/shader_module_reflection.cpp
  gct/shader_module.cpp
  gct/shader.cpp
  gct/shader_create_info.cpp
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
  gct/load_nnef_data.cpp
  gct/load_image.cpp
  gct/copy_buffer.cpp
  gct/copy_image.cpp
  gct/copy_buffer_to_image.cpp
  gct/copy_image_to_buffer.cpp
  gct/fill.cpp
  gct/blit.cpp
  gct/barrier.cpp
  gct/barrier_config.cpp
  gct/build_acceleration_structure.cpp
  gct/present_info.cpp
  gct/wait_for_sync.cpp
  gct/bind_descriptor_set.cpp
  gct/bind_pipeline.cpp
  gct/bind_vertex_buffer.cpp
  gct/bind.cpp
  gct/format_to_aspect.cpp
  gct/format_to_size.cpp
  gct/format_to_type.cpp
  gct/format_to_channels.cpp
  gct/is_available_for.cpp
  gct/get_all_formats.cpp
  gct/pixel_per_element_x.cpp
  gct/pixel_per_element_y.cpp
  gct/is_rgba.cpp
  gct/is_bgra.cpp
  gct/is_pot.cpp
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
  gct/alignment.cpp
  gct/spv_member_pointer.cpp
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
  gct/validation_cache_create_info.cpp
  gct/validation_cache.cpp
  gct/query_pool_create_info.cpp
  gct/query_pool.cpp
  gct/conditional_rendering_begin_info.cpp
  gct/draw.cpp
  gct/dispatch.cpp
  gct/font.cpp
  gct/display_mode_create_info.cpp
  gct/display_mode.cpp
  gct/display_surface_create_info.cpp
  gct/dpi.cpp
  gct/html_color.cpp
  gct/eotf.cpp
  gct/color_space.cpp
  gct/shadow_map.cpp
  gct/cubemap.cpp
  gct/gbuffer.cpp
  gct/gbuffer_create_info.cpp
  gct/voxel_image.cpp
  gct/voxel_buffer.cpp
  gct/voxel_framebuffer.cpp
  gct/distance_field.cpp
  gct/distance_field_create_info.cpp
  gct/distance_field2.cpp
  gct/distance_field2_create_info.cpp
  gct/spectrum.cpp
  gct/canvas/rect_renderer.cpp
  gct/canvas/text_renderer.cpp
  gct/canvas/canvas.cpp
  gct/sched/coroutine.cpp 
  gct/sched/thread_pool.cpp 
  gct/sched/wait.cpp
  gct/sched/epoll_notifier.cpp
  gct/primitives/triangle.cpp
  gct/primitives/plane.cpp
  gct/primitives/cube.cpp
  gct/primitives/sphere.cpp
  gct/primitives/cursor.cpp
  gct/nnef_data.cpp
  gct/numeric_type_to_oiio_type.cpp
  gct/cubemap_matrix_generator.cpp
  gct/image_io_dimension.cpp
  gct/dynamic_size_image_allocation_info.cpp
  gct/image_io_create_info.cpp
  gct/image_io.cpp
  gct/image_fill_create_info.cpp
  gct/image_blit_create_info.cpp
  gct/image_mode.cpp
  gct/image_generator_type.cpp
  gct/shader_graph_subedge.cpp
  gct/shader_graph_barrier.cpp
  gct/shader_graph_vertex_command.cpp
  gct/shader_graph_command.cpp
  gct/shader_graph.cpp
  gct/image_filter.cpp
  gct/image_filter_create_info.cpp
  gct/image_filter2_create_info.cpp
  gct/named_resource.cpp
  gct/compute.cpp
  gct/compute_create_info.cpp
  gct/graphics.cpp
  gct/graphics_create_info.cpp
  gct/tone_mapping_create_info.cpp
  gct/tone_mapping.cpp
  gct/shader_set.cpp
  gct/gauss_create_info.cpp
  gct/gauss.cpp
  gct/hbao_create_info.cpp
  gct/hbao.cpp
  gct/hbao2_create_info.cpp
  gct/hbao2.cpp
  gct/rtao_create_info.cpp
  gct/rtao.cpp
  gct/lrlf_dof_create_info.cpp
  gct/lrlf_dof.cpp
  gct/epipolar_mesh_create_info.cpp
  gct/epipolar_mesh.cpp
  gct/epipolar_light_scattering_create_info.cpp
  gct/epipolar_light_scattering.cpp
  gct/skyview_create_info.cpp
  gct/skyview.cpp
  gct/skyview_froxel_create_info.cpp
  gct/skyview_froxel_param.cpp
  gct/skyview_froxel.cpp
  gct/skyview_froxel2_create_info.cpp
  gct/skyview_froxel2_param.cpp
  gct/skyview_froxel2.cpp
  gct/nohc_benchmark_create_info.cpp
  gct/nohc_benchmark.cpp
  gct/occlusion_query_create_info.cpp
  gct/occlusion_query.cpp
  gct/lens_flare_create_info.cpp
  gct/lens_flare.cpp
  gct/draw_line_create_info.cpp
  gct/draw_line.cpp
  gct/generate_projection_matrix.cpp
  gct/projection_matrix.cpp
  gct/particle_buffer.cpp
  gct/draw_particle.cpp
  gct/draw_particle_create_info.cpp
  gct/load_obj_particle.cpp
  gct/remove_chain.cpp
  gct/index_range.cpp
  gct/matrix.cpp
  gct/matrix_pool.cpp
  gct/matrix_pool_create_info.cpp
  gct/aabb_pool.cpp
  gct/aabb_pool_create_info.cpp
  gct/punctual_light_type.cpp
  gct/light_pool_create_info.cpp
  gct/light_pool.cpp
  gct/linear_allocator.cpp
  gct/linear_allocator_create_info.cpp
  gct/texture_pool_create_info.cpp
  gct/texture_pool.cpp
  gct/buffer_pool_create_info.cpp
  gct/buffer_pool.cpp
  gct/sampler_pool_create_info.cpp
  gct/sampler_pool.cpp
  gct/image_allocate_info.cpp
  gct/subview_range.cpp
  gct/image_pool_create_info.cpp
  gct/image_pool.cpp
  gct/vertex_buffer_pool_create_info.cpp
  gct/vertex_buffer_pool.cpp
  gct/buffer_window.cpp
  gct/aabb.cpp
  gct/kdtree.cpp
  gct/key_value.cpp
  gct/radix_sort_create_info.cpp
  gct/radix_sort.cpp
  gct/onesweep_create_info.cpp
  gct/onesweep.cpp
  gct/create_image_from_file.cpp
  gct/scene_graph.cpp
  gct/scene_graph_create_info.cpp
  gct/scene_graph_resource.cpp
  gct/scene_graph_primitive.cpp
  gct/scene_graph_instance.cpp
  gct/get_shader.cpp
  gct/compiled_scene_graph.cpp
  gct/compiled_aabb_scene_graph.cpp
  gct/instance_list.cpp
  gct/chc.cpp
  gct/nohc.cpp
  gct/chcplus.cpp
  gct/hierarchical_raster_occlusion_culling_create_info.cpp
  gct/hierarchical_raster_occlusion_culling.cpp
  gct/similar_matrix.cpp
  gct/generate_random_matrix.cpp
  gct/gltf2.cpp
  gct/af_state.cpp
  ../external/SPIRV-Reflect/spirv_reflect.c
)

target_link_libraries(
  gct
  Threads::Threads
  Boost::context
  Boost::system
  ${CMAKE_DL_LIBS}
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_VULKAN2JSON_vulkan2json}
)

install( TARGETS gct )

set_target_properties(
  gct
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

if( GLFW_FOUND )

add_library( gct_glfw SHARED
  gct/glfw.cpp
  gct/common_sample_setup.cpp
)

target_link_libraries(
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::system
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

endif()

if( HARFBUZZ_FOUND AND ICUUC_FOUND )

add_library( gct_text SHARED
  gct/text/layout_engine.cpp
)

set_target_properties(
  gct_text
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

target_link_libraries(
  gct_text
  Threads::Threads
  ${CMAKE_DL_LIBS}
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_HARFBUZZ_harfbuzz}
  ${pkgcfg_lib_ICUUC_icuuc}
  ${pkgcfg_lib_ICUUC_icudata}
)

install( TARGETS gct_text )

endif()

if( XKBCOMMON_FOUND AND SDBUSCPP_FOUND AND LIBINPUT_FOUND )

add_library( gct_input SHARED
  gct/input/device_monitor.cpp
  gct/input/get_system_keymap.cpp 
  gct/input/libinput.cpp 
  gct/input/xkbcommon.cpp 
  gct/input/fcitx5.cpp 
)

set_target_properties(
  gct_input
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

target_link_libraries(
  gct_input
  Threads::Threads
  ${CMAKE_DL_LIBS}
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${pkgcfg_lib_LIBINPUT_input}
  ${pkgcfg_lib_XKBCOMMON_xkbcommon}
  ${pkgcfg_lib_SDBUSCPP_sdbus-c++}
  ${pkgcfg_lib_SDBUSCPP_systemd}
)

install( TARGETS gct_input )

endif()

if( NNEF_FOUND )
add_library( gct_dnn SHARED
  gct/dnn/nnef.cpp
  gct/dnn/nnef2json.cpp
  gct/dnn/graph.cpp
  gct/dnn/linear.cpp
  gct/dnn/convolution.cpp
  gct/dnn/relu.cpp
  gct/dnn/softmax.cpp
  gct/dnn/max_pool.cpp
  gct/dnn/reshape.cpp
  gct/dnn/load_image.cpp
  gct/dnn/synset_mapping.cpp
)

target_link_libraries(
  gct_dnn
  Threads::Threads
  ${CMAKE_DL_LIBS}
  ${pkgcfg_lib_Vulkan_vulkan}
  ${OIIO_LIBRARIES}
  ${NNEF_LIBRARIES}
)

set_target_properties(
  gct_dnn
  PROPERTIES
  VERSION ${PROJECT_VERSION}
  SOVERSION 0
)

install( TARGETS gct_dnn )

endif()

add_executable( gen_cube_mat gen_cube_mat.cpp )
target_link_libraries( gen_cube_mat
  gct
  ${Boost_PROGRAM_OPTIONS_LIBRARIES}
)

add_executable( dump_spirv dump_spirv.cpp )
target_link_libraries( dump_spirv
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::context
  Boost::system
)

add_executable( dump_gpuinfo dump_gpuinfo.cpp )
target_link_libraries( dump_gpuinfo
  gct
  gct_glfw
  Threads::Threads
  Boost::program_options
  Boost::context
  Boost::system
)

if( ENABLE_EXAMPLES )

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

add_executable( test_kdtree
  example/test_kdtree.cpp
)

target_link_libraries(
  test_kdtree
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

endif()
