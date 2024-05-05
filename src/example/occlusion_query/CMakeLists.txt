add_executable( occlusion_query
  gct.cpp
)
target_compile_definitions( occlusion_query PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( occlusion_query PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  occlusion_query
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

add_shader( occlusion_query shadow_mat/shadow_mat.comp )

add_shader( occlusion_query shadow/world.frag )
add_shader( occlusion_query shadow/world.geom )
add_shader( occlusion_query shadow/world.vert )

add_shader( occlusion_query skyview/generate_transmittance.comp )
add_shader( occlusion_query skyview/generate_skyview.comp )
add_shader( occlusion_query skyview/generate_multiscat.comp )
add_shader( occlusion_query skyview/froxel.comp )
add_shader( occlusion_query skyview/render_froxel.comp )

add_shader( occlusion_query geometry/tangent.frag )
add_shader( occlusion_query geometry/tangent.vert )
add_shader( occlusion_query geometry/world.frag )
add_shader( occlusion_query geometry/world.vert )

add_shader( occlusion_query depth/world.frag )
add_shader( occlusion_query depth/world.vert )

add_shader( occlusion_query aabb/world.frag )
add_shader( occlusion_query aabb/world.geom )
add_shader( occlusion_query aabb/world.vert )

add_shader( occlusion_query aabb_test/world.frag )
add_shader( occlusion_query aabb_test/world.geom )
add_shader( occlusion_query aabb_test/world.vert )

add_shader( occlusion_query matrix_pool/write.comp )
add_shader( occlusion_query matrix_pool/read.comp )
add_shader( occlusion_query matrix_pool/update.comp )

add_shader( occlusion_query aabb_pool/write.comp )
add_shader( occlusion_query aabb_pool/read.comp )
add_shader( occlusion_query aabb_pool/update.comp )

add_shader( occlusion_query primitive_resource_index_pool/write.comp )
add_shader( occlusion_query primitive_resource_index_pool/read.comp )

add_shader( occlusion_query instance_resource_index_pool/write.comp )
add_shader( occlusion_query instance_resource_index_pool/read.comp )

add_shader( occlusion_query visibility_pool/write.comp )
add_shader( occlusion_query visibility_pool/read.comp )

add_shader( occlusion_query gauss/h5_16.comp )
add_shader( occlusion_query gauss/v5_16.comp )
add_shader( occlusion_query gauss/h12_32.comp )
add_shader( occlusion_query gauss/v12_32.comp )

add_shader( occlusion_query lighting/lighting.comp )
add_shader( occlusion_query gamma/gamma.comp )

add_shader( occlusion_query ao/hbao_0.comp )
add_shader( occlusion_query ao/hbao_45.comp )
add_shader( occlusion_query ao/hbao_90.comp )
add_shader( occlusion_query ao/hbao_135.comp )
add_shader( occlusion_query ao/hbao.comp )

add_shader( occlusion_query mix_ao/mix_ao.comp )

add_shader( occlusion_query tone/tone.comp )
