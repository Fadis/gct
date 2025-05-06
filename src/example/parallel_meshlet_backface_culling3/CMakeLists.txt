add_executable( parallel_meshlet_backface_culling3
  gct.cpp
)
target_compile_definitions( parallel_meshlet_backface_culling3 PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( parallel_meshlet_backface_culling3 PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  parallel_meshlet_backface_culling3
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

add_shader( parallel_meshlet_backface_culling3 shadow_mat/shadow_mat.comp )



add_shader( parallel_meshlet_backface_culling3 shadow/world.frag )
add_shader( parallel_meshlet_backface_culling3 shadow/world.geom )
add_shader( parallel_meshlet_backface_culling3 shadow/world.vert )

add_shader( parallel_meshlet_backface_culling3 skyview/generate_transmittance.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/generate_skyview.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/generate_multiscat.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/generate.comp )
add_shader( parallel_meshlet_backface_culling3 skyview/render.comp )

add_shader( parallel_meshlet_backface_culling3 geometry/geometry.task )
add_shader( parallel_meshlet_backface_culling3 geometry/geometry.mesh )
add_shader( parallel_meshlet_backface_culling3 geometry/geometry.frag )
add_shader( parallel_meshlet_backface_culling3 geometry/dummy.comp )

add_shader( parallel_meshlet_backface_culling3 aabb/world.frag )
add_shader( parallel_meshlet_backface_culling3 aabb/world.geom )
add_shader( parallel_meshlet_backface_culling3 aabb/world.vert )

add_shader( parallel_meshlet_backface_culling3 depth/world.frag )
add_shader( parallel_meshlet_backface_culling3 depth/world.vert )

add_shader( parallel_meshlet_backface_culling3 matrix_pool/write.comp )
add_shader( parallel_meshlet_backface_culling3 matrix_pool/read.comp )
add_shader( parallel_meshlet_backface_culling3 matrix_pool/update.comp )
add_shader( parallel_meshlet_backface_culling3 matrix_pool/copy.comp )

add_shader( parallel_meshlet_backface_culling3 aabb_pool/write.comp )
add_shader( parallel_meshlet_backface_culling3 aabb_pool/read.comp )
add_shader( parallel_meshlet_backface_culling3 aabb_pool/update.comp )

add_shader( parallel_meshlet_backface_culling3 image_pool/rgba8.comp )
add_shader( parallel_meshlet_backface_culling3 image_pool/rgba16.comp )
add_shader( parallel_meshlet_backface_culling3 image_pool/rgba16f.comp )
add_shader( parallel_meshlet_backface_culling3 image_pool/rgba32f.comp )

add_shader( parallel_meshlet_backface_culling3 primitive_resource_index_pool/write.comp )
add_shader( parallel_meshlet_backface_culling3 primitive_resource_index_pool/read.comp )

add_shader( parallel_meshlet_backface_culling3 instance_resource_index_pool/write.comp )
add_shader( parallel_meshlet_backface_culling3 instance_resource_index_pool/read.comp )

add_shader( parallel_meshlet_backface_culling3 visibility_pool/write.comp )
add_shader( parallel_meshlet_backface_culling3 visibility_pool/read.comp )

add_shader( parallel_meshlet_backface_culling3 resource_pair/write.comp )
add_shader( parallel_meshlet_backface_culling3 resource_pair/read.comp )

add_shader( parallel_meshlet_backface_culling3 light_pool/write.comp )
add_shader( parallel_meshlet_backface_culling3 light_pool/read.comp )
add_shader( parallel_meshlet_backface_culling3 light_pool/update.comp )

add_shader( parallel_meshlet_backface_culling3 accessor/write.comp )
add_shader( parallel_meshlet_backface_culling3 accessor/read.comp )

add_shader( parallel_meshlet_backface_culling3 mesh/write.comp )
add_shader( parallel_meshlet_backface_culling3 mesh/read.comp )

add_shader( parallel_meshlet_backface_culling3 meshlet/write.comp )
add_shader( parallel_meshlet_backface_culling3 meshlet/read.comp )

add_shader( parallel_meshlet_backface_culling3 meshlet_index/write.comp )
add_shader( parallel_meshlet_backface_culling3 meshlet_index/read.comp )

add_shader( parallel_meshlet_backface_culling3 gauss/horizontal.comp )
add_shader( parallel_meshlet_backface_culling3 gauss/vertical.comp )

add_shader( parallel_meshlet_backface_culling3 lighting/lighting.comp )
add_shader( parallel_meshlet_backface_culling3 gamma/gamma.comp )

add_shader( parallel_meshlet_backface_culling3 nearest_position/nearest_position.comp )

add_shader( parallel_meshlet_backface_culling3 ao/hbao_0.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao_45.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao_90.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao_135.comp )
add_shader( parallel_meshlet_backface_culling3 ao/hbao.comp )

add_shader( parallel_meshlet_backface_culling3 mix_ao/mix_ao.comp )

add_shader( parallel_meshlet_backface_culling3 tone/tone.comp )


add_shader( parallel_meshlet_backface_culling3 distance/distance.comp )

add_shader( parallel_meshlet_backface_culling3 dof/h0.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h1.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h2.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h3.comp )
add_shader( parallel_meshlet_backface_culling3 dof/h4.comp )
add_shader( parallel_meshlet_backface_culling3 dof/v.comp )

add_shader( parallel_meshlet_backface_culling3 merge/merge.comp )

add_shader( parallel_meshlet_backface_culling3 coc_gauss/horizontal.comp )
add_shader( parallel_meshlet_backface_culling3 coc_gauss/vertical.comp )

add_shader( parallel_meshlet_backface_culling3 sb/world.vert )
add_shader( parallel_meshlet_backface_culling3 sb/world.geom )
add_shader( parallel_meshlet_backface_culling3 sb/world.frag )
add_shader( parallel_meshlet_backface_culling3 flare/world.vert )
add_shader( parallel_meshlet_backface_culling3 flare/world.geom )
add_shader( parallel_meshlet_backface_culling3 flare/world.frag )

add_shader( parallel_meshlet_backface_culling3 meshlet_normal/meshlet_normal.comp )

