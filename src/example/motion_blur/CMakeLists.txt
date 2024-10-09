add_executable( motion_blur
  gct.cpp
)
target_compile_definitions( motion_blur PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( motion_blur PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  motion_blur
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

add_shader( motion_blur shadow_mat/shadow_mat.comp )



add_shader( motion_blur shadow/world.frag )
add_shader( motion_blur shadow/world.geom )
add_shader( motion_blur shadow/world.vert )

add_shader( motion_blur skyview/generate_transmittance.comp )
add_shader( motion_blur skyview/generate_skyview.comp )
add_shader( motion_blur skyview/generate_multiscat.comp )
add_shader( motion_blur skyview/froxel.comp )
add_shader( motion_blur skyview/render_froxel.comp )

add_shader( motion_blur geometry/tangent.frag )
add_shader( motion_blur geometry/tangent.vert )
add_shader( motion_blur geometry/world.frag )
add_shader( motion_blur geometry/world.vert )
add_shader( motion_blur geometry/dummy.comp )

add_shader( motion_blur aabb/world.frag )
add_shader( motion_blur aabb/world.geom )
add_shader( motion_blur aabb/world.vert )

add_shader( motion_blur depth/world.frag )
add_shader( motion_blur depth/world.vert )

add_shader( motion_blur matrix_pool/write.comp )
add_shader( motion_blur matrix_pool/read.comp )
add_shader( motion_blur matrix_pool/update.comp )
add_shader( motion_blur matrix_pool/copy.comp )

add_shader( motion_blur aabb_pool/write.comp )
add_shader( motion_blur aabb_pool/read.comp )
add_shader( motion_blur aabb_pool/update.comp )

add_shader( motion_blur image_pool/rgba8.comp )
add_shader( motion_blur image_pool/rgba16.comp )
add_shader( motion_blur image_pool/rgba16f.comp )
add_shader( motion_blur image_pool/rgba32f.comp )

add_shader( motion_blur primitive_resource_index_pool/write.comp )
add_shader( motion_blur primitive_resource_index_pool/read.comp )

add_shader( motion_blur instance_resource_index_pool/write.comp )
add_shader( motion_blur instance_resource_index_pool/read.comp )

add_shader( motion_blur visibility_pool/write.comp )
add_shader( motion_blur visibility_pool/read.comp )

add_shader( motion_blur light_pool/write.comp )
add_shader( motion_blur light_pool/read.comp )
add_shader( motion_blur light_pool/update.comp )

add_shader( motion_blur gauss/h5_16.comp )
add_shader( motion_blur gauss/v5_16.comp )
add_shader( motion_blur gauss/h12_32.comp )
add_shader( motion_blur gauss/v12_32.comp )

add_shader( motion_blur lighting/lighting.comp )
add_shader( motion_blur gamma/gamma.comp )

add_shader( motion_blur ao/hbao_0.comp )
add_shader( motion_blur ao/hbao_45.comp )
add_shader( motion_blur ao/hbao_90.comp )
add_shader( motion_blur ao/hbao_135.comp )
add_shader( motion_blur ao/hbao.comp )

add_shader( motion_blur mix_ao/mix_ao.comp )

add_shader( motion_blur tone/tone.comp )

add_shader( motion_blur merge/merge.comp )

add_shader( motion_blur blur/blur.comp )

