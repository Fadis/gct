add_executable( sslf
  gct.cpp
)
target_compile_definitions( sslf PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( sslf PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  sslf
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

add_shader( sslf shadow_mat/shadow_mat.comp )



add_shader( sslf shadow/world.frag )
add_shader( sslf shadow/world.geom )
add_shader( sslf shadow/world.vert )

add_shader( sslf skyview/generate_transmittance.comp )
add_shader( sslf skyview/generate_skyview.comp )
add_shader( sslf skyview/generate_multiscat.comp )
add_shader( sslf skyview/froxel.comp )
add_shader( sslf skyview/render_froxel.comp )

add_shader( sslf geometry/tangent.frag )
add_shader( sslf geometry/tangent.vert )
add_shader( sslf geometry/world.frag )
add_shader( sslf geometry/world.vert )
add_shader( sslf geometry/dummy.comp )
add_shader( sslf geometry/dummy.geom )

add_shader( sslf aabb/world.frag )
add_shader( sslf aabb/world.geom )
add_shader( sslf aabb/world.vert )

add_shader( sslf depth/world.frag )
add_shader( sslf depth/world.vert )

add_shader( sslf matrix_pool/write.comp )
add_shader( sslf matrix_pool/read.comp )
add_shader( sslf matrix_pool/update.comp )
add_shader( sslf matrix_pool/copy.comp )

add_shader( sslf aabb_pool/write.comp )
add_shader( sslf aabb_pool/read.comp )
add_shader( sslf aabb_pool/update.comp )

add_shader( sslf image_pool/rgba8.comp )
add_shader( sslf image_pool/rgba16.comp )
add_shader( sslf image_pool/rgba16f.comp )
add_shader( sslf image_pool/rgba32f.comp )

add_shader( sslf primitive_resource_index_pool/write.comp )
add_shader( sslf primitive_resource_index_pool/read.comp )

add_shader( sslf instance_resource_index_pool/write.comp )
add_shader( sslf instance_resource_index_pool/read.comp )

add_shader( sslf visibility_pool/write.comp )
add_shader( sslf visibility_pool/read.comp )

add_shader( sslf resource_pair/write.comp )
add_shader( sslf resource_pair/read.comp )

add_shader( sslf light_pool/write.comp )
add_shader( sslf light_pool/read.comp )
add_shader( sslf light_pool/update.comp )

add_shader( sslf gauss/h5_16.comp )
add_shader( sslf gauss/v5_16.comp )
add_shader( sslf gauss/h12_32.comp )
add_shader( sslf gauss/v12_32.comp )

add_shader( sslf lighting/lighting.comp )
add_shader( sslf gamma/gamma.comp )

add_shader( sslf ao/hbao_0.comp )
add_shader( sslf ao/hbao_45.comp )
add_shader( sslf ao/hbao_90.comp )
add_shader( sslf ao/hbao_135.comp )
add_shader( sslf ao/hbao.comp )

add_shader( sslf mix_ao/mix_ao.comp )

add_shader( sslf tone/tone.comp )

add_shader( sslf sb/world.vert )
add_shader( sslf sb/world.geom )
add_shader( sslf sb/world.frag )
add_shader( sslf merge/merge.comp )
add_shader( sslf flare/flare.comp )
