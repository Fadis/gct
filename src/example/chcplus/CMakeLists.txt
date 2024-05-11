add_executable( chcplus
  gct.cpp
)
target_compile_definitions( chcplus PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( chcplus PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  chcplus
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

add_shader( chcplus shadow_mat/shadow_mat.comp )

add_shader( chcplus shadow/world.frag )
add_shader( chcplus shadow/world.geom )
add_shader( chcplus shadow/world.vert )

add_shader( chcplus skyview/generate_transmittance.comp )
add_shader( chcplus skyview/generate_skyview.comp )
add_shader( chcplus skyview/generate_multiscat.comp )
add_shader( chcplus skyview/froxel.comp )
add_shader( chcplus skyview/render_froxel.comp )

add_shader( chcplus geometry/tangent.frag )
add_shader( chcplus geometry/tangent.vert )
add_shader( chcplus geometry/world.frag )
add_shader( chcplus geometry/world.vert )

add_shader( chcplus depth/world.frag )
add_shader( chcplus depth/world.vert )

add_shader( chcplus aabb/world.frag )
add_shader( chcplus aabb/world.geom )
add_shader( chcplus aabb/world.vert )

add_shader( chcplus line/world.frag )
add_shader( chcplus line/world.vert )

add_shader( chcplus aabb_test/world.frag )
add_shader( chcplus aabb_test/world.geom )
add_shader( chcplus aabb_test/world.vert )

add_shader( chcplus matrix_pool/write.comp )
add_shader( chcplus matrix_pool/read.comp )
add_shader( chcplus matrix_pool/update.comp )

add_shader( chcplus aabb_pool/write.comp )
add_shader( chcplus aabb_pool/read.comp )
add_shader( chcplus aabb_pool/update.comp )

add_shader( chcplus primitive_resource_index_pool/write.comp )
add_shader( chcplus primitive_resource_index_pool/read.comp )

add_shader( chcplus instance_resource_index_pool/write.comp )
add_shader( chcplus instance_resource_index_pool/read.comp )

add_shader( chcplus visibility_pool/write.comp )
add_shader( chcplus visibility_pool/read.comp )

add_shader( chcplus gauss/h5_16.comp )
add_shader( chcplus gauss/v5_16.comp )
add_shader( chcplus gauss/h12_32.comp )
add_shader( chcplus gauss/v12_32.comp )

add_shader( chcplus lighting/lighting.comp )
add_shader( chcplus gamma/gamma.comp )

add_shader( chcplus ao/hbao_0.comp )
add_shader( chcplus ao/hbao_45.comp )
add_shader( chcplus ao/hbao_90.comp )
add_shader( chcplus ao/hbao_135.comp )
add_shader( chcplus ao/hbao.comp )

add_shader( chcplus mix_ao/mix_ao.comp )

add_shader( chcplus tone/tone.comp )
