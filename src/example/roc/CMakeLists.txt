add_executable( roc
  gct.cpp
)
target_compile_definitions( roc PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( roc PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  roc
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

add_shader( roc shadow_mat/shadow_mat.comp )



add_shader( roc shadow/world.frag )
add_shader( roc shadow/world.geom )
add_shader( roc shadow/world.vert )

add_shader( roc skyview/generate_transmittance.comp )
add_shader( roc skyview/generate_skyview.comp )
add_shader( roc skyview/generate_multiscat.comp )
add_shader( roc skyview/froxel.comp )
add_shader( roc skyview/render_froxel.comp )

add_shader( roc geometry/tangent.frag )
add_shader( roc geometry/tangent.vert )
add_shader( roc geometry/world.frag )
add_shader( roc geometry/world.vert )

add_shader( roc aabb/world.frag )
add_shader( roc aabb/world.geom )
add_shader( roc aabb/world.vert )

add_shader( roc depth/world.frag )
add_shader( roc depth/world.vert )

add_shader( roc geometry_test/world.frag )
add_shader( roc geometry_test/world.vert )
add_shader( roc geometry_test/world.geom )


add_shader( roc matrix_pool/write.comp )
add_shader( roc matrix_pool/read.comp )
add_shader( roc matrix_pool/update.comp )

add_shader( roc aabb_pool/write.comp )
add_shader( roc aabb_pool/read.comp )
add_shader( roc aabb_pool/update.comp )

add_shader( roc primitive_resource_index_pool/write.comp )
add_shader( roc primitive_resource_index_pool/read.comp )

add_shader( roc instance_resource_index_pool/write.comp )
add_shader( roc instance_resource_index_pool/read.comp )

add_shader( roc visibility_pool/write.comp )
add_shader( roc visibility_pool/read.comp )

add_shader( roc resource_pair/write.comp )
add_shader( roc resource_pair/read.comp )

add_shader( roc gauss/h5_16.comp )
add_shader( roc gauss/v5_16.comp )
add_shader( roc gauss/h12_32.comp )
add_shader( roc gauss/v12_32.comp )

add_shader( roc lighting/lighting.comp )
add_shader( roc gamma/gamma.comp )

add_shader( roc ao/hbao_0.comp )
add_shader( roc ao/hbao_45.comp )
add_shader( roc ao/hbao_90.comp )
add_shader( roc ao/hbao_135.comp )
add_shader( roc ao/hbao.comp )

add_shader( roc mix_ao/mix_ao.comp )

add_shader( roc tone/tone.comp )
