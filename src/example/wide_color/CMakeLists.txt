add_executable( wide_color
  gct.cpp
)
target_compile_definitions( wide_color PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( wide_color PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  wide_color
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

add_shader( wide_color shadow_mat/shadow_mat.comp )



add_shader( wide_color shadow/world.frag )
add_shader( wide_color shadow/world.geom )
add_shader( wide_color shadow/world.vert )

add_shader( wide_color skyview/generate_transmittance.comp )
add_shader( wide_color skyview/generate_skyview.comp )
add_shader( wide_color skyview/generate_multiscat.comp )
add_shader( wide_color skyview/froxel.comp )
add_shader( wide_color skyview/render_froxel.comp )

add_shader( wide_color geometry/tangent.frag )
add_shader( wide_color geometry/tangent.vert )
add_shader( wide_color geometry/world.frag )
add_shader( wide_color geometry/world.vert )
add_shader( wide_color geometry/dummy.comp )

add_shader( wide_color aabb/world.frag )
add_shader( wide_color aabb/world.geom )
add_shader( wide_color aabb/world.vert )

add_shader( wide_color depth/world.frag )
add_shader( wide_color depth/world.vert )

add_shader( wide_color geometry_test/world.frag )
add_shader( wide_color geometry_test/world.vert )
add_shader( wide_color geometry_test/world.geom )


add_shader( wide_color matrix_pool/write.comp )
add_shader( wide_color matrix_pool/read.comp )
add_shader( wide_color matrix_pool/update.comp )

add_shader( wide_color aabb_pool/write.comp )
add_shader( wide_color aabb_pool/read.comp )
add_shader( wide_color aabb_pool/update.comp )

add_shader( wide_color image_pool/rgba8.comp )
add_shader( wide_color image_pool/rgba16.comp )
add_shader( wide_color image_pool/rgba16f.comp )
add_shader( wide_color image_pool/rgba32f.comp )

add_shader( wide_color primitive_resource_index_pool/write.comp )
add_shader( wide_color primitive_resource_index_pool/read.comp )

add_shader( wide_color instance_resource_index_pool/write.comp )
add_shader( wide_color instance_resource_index_pool/read.comp )

add_shader( wide_color visibility_pool/write.comp )
add_shader( wide_color visibility_pool/read.comp )

add_shader( wide_color gauss/h5_16.comp )
add_shader( wide_color gauss/v5_16.comp )
add_shader( wide_color gauss/h12_32.comp )
add_shader( wide_color gauss/v12_32.comp )

add_shader( wide_color lighting/lighting.comp )
add_shader( wide_color gamma/gamma.comp )

add_shader( wide_color ao/hbao_0.comp )
add_shader( wide_color ao/hbao_45.comp )
add_shader( wide_color ao/hbao_90.comp )
add_shader( wide_color ao/hbao_135.comp )
add_shader( wide_color ao/hbao.comp )

add_shader( wide_color mix_ao/mix_ao.comp )

add_shader( wide_color tone/tone.comp )
