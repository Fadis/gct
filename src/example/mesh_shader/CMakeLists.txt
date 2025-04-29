add_executable( mesh_shader
  gct.cpp
)
target_compile_definitions( mesh_shader PRIVATE -DCMAKE_CURRENT_BINARY_DIR="${CMAKE_CURRENT_BINARY_DIR}" )
target_compile_definitions( mesh_shader PRIVATE -DCMAKE_CURRENT_SOURCE_DIR="${CMAKE_CURRENT_SOURCE_DIR}" )

target_link_libraries(
  mesh_shader
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

add_shader( mesh_shader shadow_mat/shadow_mat.comp )



add_shader( mesh_shader shadow/world.frag )
add_shader( mesh_shader shadow/world.geom )
add_shader( mesh_shader shadow/world.vert )

add_shader( mesh_shader skyview/generate_transmittance.comp )
add_shader( mesh_shader skyview/generate_skyview.comp )
add_shader( mesh_shader skyview/generate_multiscat.comp )
add_shader( mesh_shader skyview/generate.comp )
add_shader( mesh_shader skyview/render.comp )

add_shader( mesh_shader geometry/geometry.task )
add_shader( mesh_shader geometry/geometry.mesh )
add_shader( mesh_shader geometry/geometry.frag )
add_shader( mesh_shader geometry/dummy.comp )

add_shader( mesh_shader aabb/world.frag )
add_shader( mesh_shader aabb/world.geom )
add_shader( mesh_shader aabb/world.vert )

add_shader( mesh_shader depth/world.frag )
add_shader( mesh_shader depth/world.vert )

add_shader( mesh_shader matrix_pool/write.comp )
add_shader( mesh_shader matrix_pool/read.comp )
add_shader( mesh_shader matrix_pool/update.comp )
add_shader( mesh_shader matrix_pool/copy.comp )

add_shader( mesh_shader aabb_pool/write.comp )
add_shader( mesh_shader aabb_pool/read.comp )
add_shader( mesh_shader aabb_pool/update.comp )

add_shader( mesh_shader image_pool/rgba8.comp )
add_shader( mesh_shader image_pool/rgba16.comp )
add_shader( mesh_shader image_pool/rgba16f.comp )
add_shader( mesh_shader image_pool/rgba32f.comp )

add_shader( mesh_shader primitive_resource_index_pool/write.comp )
add_shader( mesh_shader primitive_resource_index_pool/read.comp )

add_shader( mesh_shader instance_resource_index_pool/write.comp )
add_shader( mesh_shader instance_resource_index_pool/read.comp )

add_shader( mesh_shader visibility_pool/write.comp )
add_shader( mesh_shader visibility_pool/read.comp )

add_shader( mesh_shader resource_pair/write.comp )
add_shader( mesh_shader resource_pair/read.comp )

add_shader( mesh_shader light_pool/write.comp )
add_shader( mesh_shader light_pool/read.comp )
add_shader( mesh_shader light_pool/update.comp )

add_shader( mesh_shader accessor/write.comp )
add_shader( mesh_shader accessor/read.comp )

add_shader( mesh_shader mesh/write.comp )
add_shader( mesh_shader mesh/read.comp )

add_shader( mesh_shader gauss/horizontal.comp )
add_shader( mesh_shader gauss/vertical.comp )

add_shader( mesh_shader lighting/lighting.comp )
add_shader( mesh_shader gamma/gamma.comp )

add_shader( mesh_shader nearest_position/nearest_position.comp )

add_shader( mesh_shader ao/hbao_0.comp )
add_shader( mesh_shader ao/hbao_45.comp )
add_shader( mesh_shader ao/hbao_90.comp )
add_shader( mesh_shader ao/hbao_135.comp )
add_shader( mesh_shader ao/hbao.comp )

add_shader( mesh_shader mix_ao/mix_ao.comp )

add_shader( mesh_shader tone/tone.comp )


add_shader( mesh_shader distance/distance.comp )

add_shader( mesh_shader dof/h0.comp )
add_shader( mesh_shader dof/h1.comp )
add_shader( mesh_shader dof/h2.comp )
add_shader( mesh_shader dof/h3.comp )
add_shader( mesh_shader dof/h4.comp )
add_shader( mesh_shader dof/v.comp )

add_shader( mesh_shader merge/merge.comp )

add_shader( mesh_shader coc_gauss/horizontal.comp )
add_shader( mesh_shader coc_gauss/vertical.comp )

add_shader( mesh_shader sb/world.vert )
add_shader( mesh_shader sb/world.geom )
add_shader( mesh_shader sb/world.frag )
add_shader( mesh_shader flare/world.vert )
add_shader( mesh_shader flare/world.geom )
add_shader( mesh_shader flare/world.frag )
