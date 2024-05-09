add_executable( nohc
  gct.cpp
)
target_compile_definitions( nohc PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( nohc PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  nohc
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

add_shader( nohc shadow_mat/shadow_mat.comp )

add_shader( nohc shadow/world.frag )
add_shader( nohc shadow/world.geom )
add_shader( nohc shadow/world.vert )

add_shader( nohc skyview/generate_transmittance.comp )
add_shader( nohc skyview/generate_skyview.comp )
add_shader( nohc skyview/generate_multiscat.comp )
add_shader( nohc skyview/froxel.comp )
add_shader( nohc skyview/render_froxel.comp )

add_shader( nohc geometry/tangent.frag )
add_shader( nohc geometry/tangent.vert )
add_shader( nohc geometry/world.frag )
add_shader( nohc geometry/world.vert )

add_shader( nohc depth/world.frag )
add_shader( nohc depth/world.vert )

add_shader( nohc aabb/world.frag )
add_shader( nohc aabb/world.geom )
add_shader( nohc aabb/world.vert )

add_shader( nohc aabb_test/world.frag )
add_shader( nohc aabb_test/world.geom )
add_shader( nohc aabb_test/world.vert )

add_shader( nohc matrix_pool/write.comp )
add_shader( nohc matrix_pool/read.comp )
add_shader( nohc matrix_pool/update.comp )

add_shader( nohc aabb_pool/write.comp )
add_shader( nohc aabb_pool/read.comp )
add_shader( nohc aabb_pool/update.comp )

add_shader( nohc primitive_resource_index_pool/write.comp )
add_shader( nohc primitive_resource_index_pool/read.comp )

add_shader( nohc instance_resource_index_pool/write.comp )
add_shader( nohc instance_resource_index_pool/read.comp )

add_shader( nohc visibility_pool/write.comp )
add_shader( nohc visibility_pool/read.comp )

add_shader( nohc gauss/h5_16.comp )
add_shader( nohc gauss/v5_16.comp )
add_shader( nohc gauss/h12_32.comp )
add_shader( nohc gauss/v12_32.comp )

add_shader( nohc lighting/lighting.comp )
add_shader( nohc gamma/gamma.comp )

add_shader( nohc ao/hbao_0.comp )
add_shader( nohc ao/hbao_45.comp )
add_shader( nohc ao/hbao_90.comp )
add_shader( nohc ao/hbao_135.comp )
add_shader( nohc ao/hbao.comp )

add_shader( nohc mix_ao/mix_ao.comp )

add_shader( nohc tone/tone.comp )
