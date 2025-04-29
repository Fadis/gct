add_executable( gauss_dof
  gct.cpp
)
target_compile_definitions( gauss_dof PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( gauss_dof PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  gauss_dof
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

add_shader( gauss_dof shadow_mat/shadow_mat.comp )



add_shader( gauss_dof shadow/world.frag )
add_shader( gauss_dof shadow/world.geom )
add_shader( gauss_dof shadow/world.vert )

add_shader( gauss_dof skyview/generate_transmittance.comp )
add_shader( gauss_dof skyview/generate_skyview.comp )
add_shader( gauss_dof skyview/generate_multiscat.comp )
add_shader( gauss_dof skyview/froxel.comp )
add_shader( gauss_dof skyview/render_froxel.comp )

add_shader( gauss_dof geometry/tangent.frag )
add_shader( gauss_dof geometry/tangent.vert )
add_shader( gauss_dof geometry/world.frag )
add_shader( gauss_dof geometry/world.vert )
add_shader( gauss_dof geometry/dummy.comp )

add_shader( gauss_dof aabb/world.frag )
add_shader( gauss_dof aabb/world.geom )
add_shader( gauss_dof aabb/world.vert )

add_shader( gauss_dof depth/world.frag )
add_shader( gauss_dof depth/world.vert )

add_shader( gauss_dof matrix_pool/write.comp )
add_shader( gauss_dof matrix_pool/read.comp )
add_shader( gauss_dof matrix_pool/update.comp )
add_shader( gauss_dof matrix_pool/copy.comp )

add_shader( gauss_dof aabb_pool/write.comp )
add_shader( gauss_dof aabb_pool/read.comp )
add_shader( gauss_dof aabb_pool/update.comp )

add_shader( gauss_dof image_pool/rgba8.comp )
add_shader( gauss_dof image_pool/rgba16.comp )
add_shader( gauss_dof image_pool/rgba16f.comp )
add_shader( gauss_dof image_pool/rgba32f.comp )

add_shader( gauss_dof primitive_resource_index_pool/write.comp )
add_shader( gauss_dof primitive_resource_index_pool/read.comp )

add_shader( gauss_dof instance_resource_index_pool/write.comp )
add_shader( gauss_dof instance_resource_index_pool/read.comp )

add_shader( gauss_dof visibility_pool/write.comp )
add_shader( gauss_dof visibility_pool/read.comp )

add_shader( gauss_dof resource_pair/write.comp )
add_shader( gauss_dof resource_pair/read.comp )

add_shader( gauss_dof light_pool/write.comp )
add_shader( gauss_dof light_pool/read.comp )
add_shader( gauss_dof light_pool/update.comp )

add_shader( gauss_dof gauss/h5_16.comp )
add_shader( gauss_dof gauss/v5_16.comp )
add_shader( gauss_dof gauss/h12_32.comp )
add_shader( gauss_dof gauss/v12_32.comp )

add_shader( gauss_dof lighting/lighting.comp )
add_shader( gauss_dof gamma/gamma.comp )

add_shader( gauss_dof ao/hbao_0.comp )
add_shader( gauss_dof ao/hbao_45.comp )
add_shader( gauss_dof ao/hbao_90.comp )
add_shader( gauss_dof ao/hbao_135.comp )
add_shader( gauss_dof ao/hbao.comp )

add_shader( gauss_dof mix_ao/mix_ao.comp )

add_shader( gauss_dof tone/tone.comp )

add_shader( gauss_dof distance/distance.comp )

add_shader( gauss_dof dof/hadaptive.comp )
add_shader( gauss_dof dof/vadaptive.comp )

add_shader( gauss_dof merge/merge.comp )

add_shader( gauss_dof coc_gauss/h12_32.comp )
add_shader( gauss_dof coc_gauss/v12_32.comp )
