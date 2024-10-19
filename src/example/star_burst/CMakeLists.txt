add_executable( star_burst
  gct.cpp
)
target_compile_definitions( star_burst PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( star_burst PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  star_burst
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

add_shader( star_burst shadow_mat/shadow_mat.comp )



add_shader( star_burst shadow/world.frag )
add_shader( star_burst shadow/world.geom )
add_shader( star_burst shadow/world.vert )

add_shader( star_burst skyview/generate_transmittance.comp )
add_shader( star_burst skyview/generate_skyview.comp )
add_shader( star_burst skyview/generate_multiscat.comp )
add_shader( star_burst skyview/froxel.comp )
add_shader( star_burst skyview/render_froxel.comp )

add_shader( star_burst geometry/tangent.frag )
add_shader( star_burst geometry/tangent.vert )
add_shader( star_burst geometry/world.frag )
add_shader( star_burst geometry/world.vert )
add_shader( star_burst geometry/dummy.comp )
add_shader( star_burst geometry/dummy.geom )

add_shader( star_burst aabb/world.frag )
add_shader( star_burst aabb/world.geom )
add_shader( star_burst aabb/world.vert )

add_shader( star_burst depth/world.frag )
add_shader( star_burst depth/world.vert )

add_shader( star_burst matrix_pool/write.comp )
add_shader( star_burst matrix_pool/read.comp )
add_shader( star_burst matrix_pool/update.comp )
add_shader( star_burst matrix_pool/copy.comp )

add_shader( star_burst aabb_pool/write.comp )
add_shader( star_burst aabb_pool/read.comp )
add_shader( star_burst aabb_pool/update.comp )

add_shader( star_burst image_pool/rgba8.comp )
add_shader( star_burst image_pool/rgba16.comp )
add_shader( star_burst image_pool/rgba16f.comp )
add_shader( star_burst image_pool/rgba32f.comp )

add_shader( star_burst primitive_resource_index_pool/write.comp )
add_shader( star_burst primitive_resource_index_pool/read.comp )

add_shader( star_burst instance_resource_index_pool/write.comp )
add_shader( star_burst instance_resource_index_pool/read.comp )

add_shader( star_burst visibility_pool/write.comp )
add_shader( star_burst visibility_pool/read.comp )

add_shader( star_burst light_pool/write.comp )
add_shader( star_burst light_pool/read.comp )
add_shader( star_burst light_pool/update.comp )

add_shader( star_burst gauss/h5_16.comp )
add_shader( star_burst gauss/v5_16.comp )
add_shader( star_burst gauss/h12_32.comp )
add_shader( star_burst gauss/v12_32.comp )

add_shader( star_burst lighting/lighting.comp )
add_shader( star_burst gamma/gamma.comp )

add_shader( star_burst ao/hbao_0.comp )
add_shader( star_burst ao/hbao_45.comp )
add_shader( star_burst ao/hbao_90.comp )
add_shader( star_burst ao/hbao_135.comp )
add_shader( star_burst ao/hbao.comp )

add_shader( star_burst mix_ao/mix_ao.comp )

add_shader( star_burst tone/tone.comp )

add_shader( star_burst sb/world.vert )
add_shader( star_burst sb/world.geom )
add_shader( star_burst sb/world.frag )

