add_executable( natural_vignetting
  gct.cpp
)
target_compile_definitions( natural_vignetting PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( natural_vignetting PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  natural_vignetting
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

add_shader( natural_vignetting shadow_mat/shadow_mat.comp )



add_shader( natural_vignetting shadow/world.frag )
add_shader( natural_vignetting shadow/world.geom )
add_shader( natural_vignetting shadow/world.vert )

add_shader( natural_vignetting skyview/generate_transmittance.comp )
add_shader( natural_vignetting skyview/generate_skyview.comp )
add_shader( natural_vignetting skyview/generate_multiscat.comp )
add_shader( natural_vignetting skyview/froxel.comp )
add_shader( natural_vignetting skyview/render_froxel.comp )

add_shader( natural_vignetting geometry/tangent.frag )
add_shader( natural_vignetting geometry/tangent.vert )
add_shader( natural_vignetting geometry/world.frag )
add_shader( natural_vignetting geometry/world.vert )
add_shader( natural_vignetting geometry/dummy.comp )
add_shader( natural_vignetting geometry/dummy.geom )

add_shader( natural_vignetting aabb/world.frag )
add_shader( natural_vignetting aabb/world.geom )
add_shader( natural_vignetting aabb/world.vert )

add_shader( natural_vignetting depth/world.frag )
add_shader( natural_vignetting depth/world.vert )

add_shader( natural_vignetting matrix_pool/write.comp )
add_shader( natural_vignetting matrix_pool/read.comp )
add_shader( natural_vignetting matrix_pool/update.comp )
add_shader( natural_vignetting matrix_pool/copy.comp )

add_shader( natural_vignetting aabb_pool/write.comp )
add_shader( natural_vignetting aabb_pool/read.comp )
add_shader( natural_vignetting aabb_pool/update.comp )

add_shader( natural_vignetting image_pool/rgba8.comp )
add_shader( natural_vignetting image_pool/rgba16.comp )
add_shader( natural_vignetting image_pool/rgba16f.comp )
add_shader( natural_vignetting image_pool/rgba32f.comp )

add_shader( natural_vignetting primitive_resource_index_pool/write.comp )
add_shader( natural_vignetting primitive_resource_index_pool/read.comp )

add_shader( natural_vignetting instance_resource_index_pool/write.comp )
add_shader( natural_vignetting instance_resource_index_pool/read.comp )

add_shader( natural_vignetting visibility_pool/write.comp )
add_shader( natural_vignetting visibility_pool/read.comp )

add_shader( natural_vignetting light_pool/write.comp )
add_shader( natural_vignetting light_pool/read.comp )
add_shader( natural_vignetting light_pool/update.comp )

add_shader( natural_vignetting gauss/h5_16.comp )
add_shader( natural_vignetting gauss/v5_16.comp )
add_shader( natural_vignetting gauss/h12_32.comp )
add_shader( natural_vignetting gauss/v12_32.comp )

add_shader( natural_vignetting lighting/lighting.comp )
add_shader( natural_vignetting gamma/gamma.comp )

add_shader( natural_vignetting ao/hbao_0.comp )
add_shader( natural_vignetting ao/hbao_45.comp )
add_shader( natural_vignetting ao/hbao_90.comp )
add_shader( natural_vignetting ao/hbao_135.comp )
add_shader( natural_vignetting ao/hbao.comp )

add_shader( natural_vignetting mix_ao/mix_ao.comp )

add_shader( natural_vignetting tone/tone.comp )

add_shader( natural_vignetting sb/world.vert )
add_shader( natural_vignetting sb/world.geom )
add_shader( natural_vignetting sb/world.frag )
add_shader( natural_vignetting merge/merge.comp )
add_shader( natural_vignetting vignetting/vignetting.comp )
