add_executable( gauss_dof2
  gct.cpp
)
target_compile_definitions( gauss_dof2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( gauss_dof2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  gauss_dof2
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

add_shader( gauss_dof2 shadow_mat/shadow_mat.comp )



add_shader( gauss_dof2 shadow/world.frag )
add_shader( gauss_dof2 shadow/world.geom )
add_shader( gauss_dof2 shadow/world.vert )

add_shader( gauss_dof2 skyview/generate_transmittance.comp )
add_shader( gauss_dof2 skyview/generate_skyview.comp )
add_shader( gauss_dof2 skyview/generate_multiscat.comp )
add_shader( gauss_dof2 skyview/froxel.comp )
add_shader( gauss_dof2 skyview/render_froxel.comp )

add_shader( gauss_dof2 geometry/tangent.frag )
add_shader( gauss_dof2 geometry/tangent.vert )
add_shader( gauss_dof2 geometry/world.frag )
add_shader( gauss_dof2 geometry/world.vert )
add_shader( gauss_dof2 geometry/dummy.comp )

add_shader( gauss_dof2 aabb/world.frag )
add_shader( gauss_dof2 aabb/world.geom )
add_shader( gauss_dof2 aabb/world.vert )

add_shader( gauss_dof2 depth/world.frag )
add_shader( gauss_dof2 depth/world.vert )

add_shader( gauss_dof2 matrix_pool/write.comp )
add_shader( gauss_dof2 matrix_pool/read.comp )
add_shader( gauss_dof2 matrix_pool/update.comp )
add_shader( gauss_dof2 matrix_pool/copy.comp )

add_shader( gauss_dof2 aabb_pool/write.comp )
add_shader( gauss_dof2 aabb_pool/read.comp )
add_shader( gauss_dof2 aabb_pool/update.comp )

add_shader( gauss_dof2 image_pool/rgba8.comp )
add_shader( gauss_dof2 image_pool/rgba16.comp )
add_shader( gauss_dof2 image_pool/rgba16f.comp )
add_shader( gauss_dof2 image_pool/rgba32f.comp )

add_shader( gauss_dof2 primitive_resource_index_pool/write.comp )
add_shader( gauss_dof2 primitive_resource_index_pool/read.comp )

add_shader( gauss_dof2 instance_resource_index_pool/write.comp )
add_shader( gauss_dof2 instance_resource_index_pool/read.comp )

add_shader( gauss_dof2 visibility_pool/write.comp )
add_shader( gauss_dof2 visibility_pool/read.comp )

add_shader( gauss_dof2 resource_pair/write.comp )
add_shader( gauss_dof2 resource_pair/read.comp )

add_shader( gauss_dof2 light_pool/write.comp )
add_shader( gauss_dof2 light_pool/read.comp )
add_shader( gauss_dof2 light_pool/update.comp )

add_shader( gauss_dof2 gauss/h5_16.comp )
add_shader( gauss_dof2 gauss/v5_16.comp )
add_shader( gauss_dof2 gauss/h12_32.comp )
add_shader( gauss_dof2 gauss/v12_32.comp )

add_shader( gauss_dof2 lighting/lighting.comp )
add_shader( gauss_dof2 gamma/gamma.comp )

add_shader( gauss_dof2 ao/hbao_0.comp )
add_shader( gauss_dof2 ao/hbao_45.comp )
add_shader( gauss_dof2 ao/hbao_90.comp )
add_shader( gauss_dof2 ao/hbao_135.comp )
add_shader( gauss_dof2 ao/hbao.comp )

add_shader( gauss_dof2 mix_ao/mix_ao.comp )

add_shader( gauss_dof2 tone/tone.comp )


add_shader( gauss_dof2 distance/distance.comp )

add_shader( gauss_dof2 dof/hadaptive.comp )
add_shader( gauss_dof2 dof/vadaptive.comp )

add_shader( gauss_dof2 merge/merge.comp )

add_shader( gauss_dof2 coc_gauss/h12_32.comp )
add_shader( gauss_dof2 coc_gauss/v12_32.comp )
