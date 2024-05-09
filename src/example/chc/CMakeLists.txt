add_executable( chc
  gct.cpp
)
target_compile_definitions( chc PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( chc PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  chc
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

add_shader( chc shadow_mat/shadow_mat.comp )

add_shader( chc shadow/world.frag )
add_shader( chc shadow/world.geom )
add_shader( chc shadow/world.vert )

add_shader( chc skyview/generate_transmittance.comp )
add_shader( chc skyview/generate_skyview.comp )
add_shader( chc skyview/generate_multiscat.comp )
add_shader( chc skyview/froxel.comp )
add_shader( chc skyview/render_froxel.comp )

add_shader( chc geometry/tangent.frag )
add_shader( chc geometry/tangent.vert )
add_shader( chc geometry/world.frag )
add_shader( chc geometry/world.vert )

add_shader( chc depth/world.frag )
add_shader( chc depth/world.vert )

add_shader( chc aabb/world.frag )
add_shader( chc aabb/world.geom )
add_shader( chc aabb/world.vert )

add_shader( chc line/world.frag )
add_shader( chc line/world.vert )

add_shader( chc aabb_test/world.frag )
add_shader( chc aabb_test/world.geom )
add_shader( chc aabb_test/world.vert )

add_shader( chc matrix_pool/write.comp )
add_shader( chc matrix_pool/read.comp )
add_shader( chc matrix_pool/update.comp )

add_shader( chc aabb_pool/write.comp )
add_shader( chc aabb_pool/read.comp )
add_shader( chc aabb_pool/update.comp )

add_shader( chc primitive_resource_index_pool/write.comp )
add_shader( chc primitive_resource_index_pool/read.comp )

add_shader( chc instance_resource_index_pool/write.comp )
add_shader( chc instance_resource_index_pool/read.comp )

add_shader( chc visibility_pool/write.comp )
add_shader( chc visibility_pool/read.comp )

add_shader( chc gauss/h5_16.comp )
add_shader( chc gauss/v5_16.comp )
add_shader( chc gauss/h12_32.comp )
add_shader( chc gauss/v12_32.comp )

add_shader( chc lighting/lighting.comp )
add_shader( chc gamma/gamma.comp )

add_shader( chc ao/hbao_0.comp )
add_shader( chc ao/hbao_45.comp )
add_shader( chc ao/hbao_90.comp )
add_shader( chc ao/hbao_135.comp )
add_shader( chc ao/hbao.comp )

add_shader( chc mix_ao/mix_ao.comp )

add_shader( chc tone/tone.comp )
