add_executable( prtlf
  gct.cpp
)
target_compile_definitions( prtlf PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( prtlf PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  prtlf
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

add_shader( prtlf shadow_mat/shadow_mat.comp )



add_shader( prtlf shadow/world.frag )
add_shader( prtlf shadow/world.geom )
add_shader( prtlf shadow/world.vert )

add_shader( prtlf skyview/generate_transmittance.comp )
add_shader( prtlf skyview/generate_skyview.comp )
add_shader( prtlf skyview/generate_multiscat.comp )
add_shader( prtlf skyview/froxel.comp )
add_shader( prtlf skyview/render_froxel.comp )

add_shader( prtlf geometry/tangent.frag )
add_shader( prtlf geometry/tangent.vert )
add_shader( prtlf geometry/world.frag )
add_shader( prtlf geometry/world.vert )
add_shader( prtlf geometry/dummy.comp )
add_shader( prtlf geometry/dummy.geom )

add_shader( prtlf aabb/world.frag )
add_shader( prtlf aabb/world.geom )
add_shader( prtlf aabb/world.vert )

add_shader( prtlf depth/world.frag )
add_shader( prtlf depth/world.vert )

add_shader( prtlf matrix_pool/write.comp )
add_shader( prtlf matrix_pool/read.comp )
add_shader( prtlf matrix_pool/update.comp )
add_shader( prtlf matrix_pool/copy.comp )

add_shader( prtlf aabb_pool/write.comp )
add_shader( prtlf aabb_pool/read.comp )
add_shader( prtlf aabb_pool/update.comp )

add_shader( prtlf image_pool/rgba8.comp )
add_shader( prtlf image_pool/rgba16.comp )
add_shader( prtlf image_pool/rgba16f.comp )
add_shader( prtlf image_pool/rgba32f.comp )

add_shader( prtlf primitive_resource_index_pool/write.comp )
add_shader( prtlf primitive_resource_index_pool/read.comp )

add_shader( prtlf instance_resource_index_pool/write.comp )
add_shader( prtlf instance_resource_index_pool/read.comp )

add_shader( prtlf visibility_pool/write.comp )
add_shader( prtlf visibility_pool/read.comp )

add_shader( prtlf light_pool/write.comp )
add_shader( prtlf light_pool/read.comp )
add_shader( prtlf light_pool/update.comp )

add_shader( prtlf gauss/h5_16.comp )
add_shader( prtlf gauss/v5_16.comp )
add_shader( prtlf gauss/h12_32.comp )
add_shader( prtlf gauss/v12_32.comp )

add_shader( prtlf lighting/lighting.comp )
add_shader( prtlf gamma/gamma.comp )

add_shader( prtlf ao/hbao_0.comp )
add_shader( prtlf ao/hbao_45.comp )
add_shader( prtlf ao/hbao_90.comp )
add_shader( prtlf ao/hbao_135.comp )
add_shader( prtlf ao/hbao.comp )

add_shader( prtlf mix_ao/mix_ao.comp )

add_shader( prtlf tone/tone.comp )
add_shader( prtlf sb/world.vert )
add_shader( prtlf sb/world.geom )
add_shader( prtlf sb/world.frag )
add_shader( prtlf flare/world.vert )
add_shader( prtlf flare/world.geom )
add_shader( prtlf flare/world.frag )
