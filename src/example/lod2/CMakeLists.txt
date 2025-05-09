add_executable( lod2
  gct.cpp
)
target_compile_definitions( lod2 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( lod2 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  lod2
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

add_shader( lod2 shadow_mat/shadow_mat.comp )



add_shader( lod2 shadow/world.frag )
add_shader( lod2 shadow/world.geom )
add_shader( lod2 shadow/world.vert )

add_shader( lod2 skyview/generate_transmittance.comp )
add_shader( lod2 skyview/generate_skyview.comp )
add_shader( lod2 skyview/generate_multiscat.comp )
add_shader( lod2 skyview/generate.comp )
add_shader( lod2 skyview/render.comp )

add_shader( lod2 geometry/geometry.task )
add_shader( lod2 geometry/geometry.mesh )
add_shader( lod2 geometry/geometry.frag )
add_shader( lod2 geometry/dummy.comp )

add_shader( lod2 morph/morph.task )
add_shader( lod2 morph/morph.mesh )
add_shader( lod2 morph/morph.frag )
add_shader( lod2 morph/mask.comp )
add_shader( lod2 morph/voronoi.comp )
add_shader( lod2 morph/fill.comp )
add_shader( lod2 morph/generate.comp )

add_shader( lod2 aabb/world.frag )
add_shader( lod2 aabb/world.geom )
add_shader( lod2 aabb/world.vert )

add_shader( lod2 depth/world.frag )
add_shader( lod2 depth/world.vert )

add_shader( lod2 matrix_pool/write.comp )
add_shader( lod2 matrix_pool/read.comp )
add_shader( lod2 matrix_pool/update.comp )
add_shader( lod2 matrix_pool/copy.comp )

add_shader( lod2 aabb_pool/write.comp )
add_shader( lod2 aabb_pool/read.comp )
add_shader( lod2 aabb_pool/update.comp )

add_shader( lod2 image_pool/rgba8.comp )
add_shader( lod2 image_pool/rgba16.comp )
add_shader( lod2 image_pool/rgba16f.comp )
add_shader( lod2 image_pool/rgba32f.comp )

add_shader( lod2 primitive_resource_index_pool/write.comp )
add_shader( lod2 primitive_resource_index_pool/read.comp )

add_shader( lod2 instance_resource_index_pool/write.comp )
add_shader( lod2 instance_resource_index_pool/read.comp )

add_shader( lod2 visibility_pool/write.comp )
add_shader( lod2 visibility_pool/read.comp )

add_shader( lod2 resource_pair/write.comp )
add_shader( lod2 resource_pair/read.comp )

add_shader( lod2 light_pool/write.comp )
add_shader( lod2 light_pool/read.comp )
add_shader( lod2 light_pool/update.comp )

add_shader( lod2 accessor/write.comp )
add_shader( lod2 accessor/read.comp )

add_shader( lod2 mesh/write.comp )
add_shader( lod2 mesh/read.comp )

add_shader( lod2 meshlet/write.comp )
add_shader( lod2 meshlet/read.comp )

add_shader( lod2 meshlet_index/write.comp )
add_shader( lod2 meshlet_index/read.comp )

add_shader( lod2 gauss/horizontal.comp )
add_shader( lod2 gauss/vertical.comp )

add_shader( lod2 lighting/lighting.comp )
add_shader( lod2 gamma/gamma.comp )

add_shader( lod2 nearest_position/nearest_position.comp )

add_shader( lod2 ao/hbao_0.comp )
add_shader( lod2 ao/hbao_45.comp )
add_shader( lod2 ao/hbao_90.comp )
add_shader( lod2 ao/hbao_135.comp )
add_shader( lod2 ao/hbao.comp )

add_shader( lod2 mix_ao/mix_ao.comp )

add_shader( lod2 tone/tone.comp )


add_shader( lod2 distance/distance.comp )

add_shader( lod2 dof/h0.comp )
add_shader( lod2 dof/h1.comp )
add_shader( lod2 dof/h2.comp )
add_shader( lod2 dof/h3.comp )
add_shader( lod2 dof/h4.comp )
add_shader( lod2 dof/v.comp )

add_shader( lod2 merge/merge.comp )

add_shader( lod2 coc_gauss/horizontal.comp )
add_shader( lod2 coc_gauss/vertical.comp )

add_shader( lod2 sb/world.vert )
add_shader( lod2 sb/world.geom )
add_shader( lod2 sb/world.frag )
add_shader( lod2 flare/world.vert )
add_shader( lod2 flare/world.geom )
add_shader( lod2 flare/world.frag )

add_shader( lod2 meshlet_normal/meshlet_normal.comp )

