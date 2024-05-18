add_executable( frustum_culling
  gct.cpp
)
target_compile_definitions( frustum_culling PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( frustum_culling PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  frustum_culling
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

add_shader( frustum_culling shadow_mat/shadow_mat.comp )



add_shader( frustum_culling shadow/world.frag )
add_shader( frustum_culling shadow/world.geom )
add_shader( frustum_culling shadow/world.vert )

add_shader( frustum_culling skyview/generate_transmittance.comp )
add_shader( frustum_culling skyview/generate_skyview.comp )
add_shader( frustum_culling skyview/generate_multiscat.comp )
add_shader( frustum_culling skyview/froxel.comp )
add_shader( frustum_culling skyview/render_froxel.comp )

add_shader( frustum_culling geometry/tangent.frag )
add_shader( frustum_culling geometry/tangent.vert )
add_shader( frustum_culling geometry/world.frag )
add_shader( frustum_culling geometry/world.vert )

add_shader( frustum_culling geometry_test/world.frag )
add_shader( frustum_culling geometry_test/world.vert )
add_shader( frustum_culling geometry_test/world.geom )

add_shader( frustum_culling line/world.vert )
add_shader( frustum_culling line/world.frag )

add_shader( frustum_culling matrix_pool/write.comp )
add_shader( frustum_culling matrix_pool/read.comp )
add_shader( frustum_culling matrix_pool/update.comp )

add_shader( frustum_culling aabb_pool/write.comp )
add_shader( frustum_culling aabb_pool/read.comp )
add_shader( frustum_culling aabb_pool/update.comp )

add_shader( frustum_culling primitive_resource_index_pool/write.comp )
add_shader( frustum_culling primitive_resource_index_pool/read.comp )

add_shader( frustum_culling instance_resource_index_pool/write.comp )
add_shader( frustum_culling instance_resource_index_pool/read.comp )

add_shader( frustum_culling visibility_pool/write.comp )
add_shader( frustum_culling visibility_pool/read.comp )

add_shader( frustum_culling gauss/h5_16.comp )
add_shader( frustum_culling gauss/v5_16.comp )
add_shader( frustum_culling gauss/h12_32.comp )
add_shader( frustum_culling gauss/v12_32.comp )

add_shader( frustum_culling lighting/lighting.comp )
add_shader( frustum_culling gamma/gamma.comp )

add_shader( frustum_culling ao/hbao_0.comp )
add_shader( frustum_culling ao/hbao_45.comp )
add_shader( frustum_culling ao/hbao_90.comp )
add_shader( frustum_culling ao/hbao_135.comp )
add_shader( frustum_culling ao/hbao.comp )

add_shader( frustum_culling mix_ao/mix_ao.comp )

add_shader( frustum_culling tone/tone.comp )
