add_executable( hroc
  gct.cpp
)
target_compile_definitions( hroc PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( hroc PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  hroc
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

add_shader( hroc shadow_mat/shadow_mat.comp )

add_shader( hroc occlusion/histgram.comp )
add_shader( hroc occlusion/histgram_sum.comp )
add_shader( hroc occlusion/local_sum.comp )
add_shader( hroc occlusion/global_sum.comp )
add_shader( hroc occlusion/sort.comp )
add_shader( hroc occlusion/small_sort.comp )
add_shader( hroc occlusion/setup_sort.comp )
add_shader( hroc occlusion/setup_leaf.comp )
add_shader( hroc occlusion/setup_node.comp )
add_shader( hroc occlusion/mark_leaf.comp )
add_shader( hroc occlusion/mark_node.comp )

add_shader( hroc shadow/world.frag )
add_shader( hroc shadow/world.geom )
add_shader( hroc shadow/world.vert )

add_shader( hroc skyview/generate_transmittance.comp )
add_shader( hroc skyview/generate_skyview.comp )
add_shader( hroc skyview/generate_multiscat.comp )
add_shader( hroc skyview/froxel.comp )
add_shader( hroc skyview/render_froxel.comp )

add_shader( hroc geometry/tangent.frag )
add_shader( hroc geometry/tangent.vert )
add_shader( hroc geometry/world.frag )
add_shader( hroc geometry/world.vert )

add_shader( hroc aabb/world.frag )
add_shader( hroc aabb/world.geom )
add_shader( hroc aabb/world.vert )

add_shader( hroc depth/world.frag )
add_shader( hroc depth/world.vert )

add_shader( hroc geometry_test/world.frag )
add_shader( hroc geometry_test/world.vert )
add_shader( hroc geometry_test/world.geom )


add_shader( hroc matrix_pool/write.comp )
add_shader( hroc matrix_pool/read.comp )
add_shader( hroc matrix_pool/update.comp )

add_shader( hroc aabb_pool/write.comp )
add_shader( hroc aabb_pool/read.comp )
add_shader( hroc aabb_pool/update.comp )

add_shader( hroc primitive_resource_index_pool/write.comp )
add_shader( hroc primitive_resource_index_pool/read.comp )

add_shader( hroc instance_resource_index_pool/write.comp )
add_shader( hroc instance_resource_index_pool/read.comp )

add_shader( hroc visibility_pool/write.comp )
add_shader( hroc visibility_pool/read.comp )

add_shader( hroc gauss/h5_16.comp )
add_shader( hroc gauss/v5_16.comp )
add_shader( hroc gauss/h12_32.comp )
add_shader( hroc gauss/v12_32.comp )

add_shader( hroc lighting/lighting.comp )
add_shader( hroc gamma/gamma.comp )

add_shader( hroc ao/hbao_0.comp )
add_shader( hroc ao/hbao_45.comp )
add_shader( hroc ao/hbao_90.comp )
add_shader( hroc ao/hbao_135.comp )
add_shader( hroc ao/hbao.comp )

add_shader( hroc mix_ao/mix_ao.comp )

add_shader( hroc tone/tone.comp )
